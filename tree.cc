#include "tree.h"  

char * pick(int);
char readline[10];
int operatorNum (char *);
double parse(char *, double, double);

char *x[101], *y[101];
const char *F[7] = {"+", "-", "*", "%", "sin", "cos", NULL};

// Loading data
void data_load()
{
	printf("load data\t\t");
	long lSize;
	size_t result;

	FILE *pFile = fopen("proj2-data.dat", "r");
	if (pFile == NULL){
		fputs("File error",stderr);		
		exit (1);
	}

	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
  	rewind (pFile);
	char *buffer;
	buffer = (char*) malloc (sizeof(char)*lSize);
	
	if (buffer ==NULL) {
		fputs("Memory error",stderr);
		exit (2);
	}
	
	result = fread(buffer, 1, lSize, pFile);
	if (result != lSize){
		fputs("Reading error",stderr);
		exit (3);
	}	
	
	char *str; 
	str = (char*) calloc (1, sizeof(char));

	int j = 0, line=0;
	
	for (int i=0; i < strlen(buffer); i++)
	{	
		if (buffer[i] != '\t' && buffer[i] != '\n')
			str[j++] = buffer[i];

		else if (buffer[i] == '\t')
		{	
			x[line] = (char*) malloc (sizeof(char));
			if (line < 30)
				strncpy(x[line], str, 4);
			else 
				strncpy(x[line], str, 3);
			j = 0;
			memset(str,0,sizeof(char)*2);
		}
		else if (buffer[i] == '\n')
		{
			y[line] = (char*) malloc (sizeof(char));
			strcpy(y[line], str);
			line++;
			j = 0;
			memset(str,0,sizeof(char)*2);
		}
		else
			printf("Unexpected char\n");
	}

	free(str);
	free(buffer);
	fclose(pFile);

	printf("......finish.\n");
}

// Constructor and destructor
tree::tree(){
	root_node = NULL;
}

tree::~tree(){
	destroy_tree(root_node);
}

// Create tree by max layer and type(full: 1, growing: 0)
void tree::initial(int depth, bool type){
	root_node = new node;
	root_node->layer = 0;
		
	create_tree(depth, root_node, type);
}

// Cal max depth of tree
int tree::maxDep_tree(node *p){
	if(p == NULL)
		return 0;
	int left_dep = maxDep_tree(p->left);
	int right_dep = maxDep_tree(p->right);
	return (left_dep > right_dep) ? left_dep + 1: right_dep + 1;
}

tree::node *tree::create_tree(int depth, node *p, bool type){
	
	// pick a symbol for node
	if (p->layer == 0)
		p->symbol = pick(1);
	else if (p->layer != 0 && p->layer == depth - 1)
		p->symbol = pick(0);
	else{
		if(type == 0){
			if(maxDep_tree(root_node) < depth)
				p->symbol = pick(1);
			else
				p->symbol = rand()%5 ? pick(1):pick(0);  // ? choose number in what percentage
		
		}
		else if(type == 1)
			p->symbol = pick(1);
		else{
			puts("Error tree type: grow or full.\n");
			exit(1);
		}
	}
	// check the type of symbol and create it's children
	if(operatorNum(p->symbol) == 0){
		p->left = NULL;
		p->right = NULL;
	}
	else if(operatorNum(p->symbol) == 1){
		p->left = new node;
		p->left->layer = p->layer + 1;
		create_tree(depth, p->left, type);
		p->right = NULL;
	}
	else{
		p->left = new node;
		p->left->layer = p->layer + 1;
		create_tree(depth, p->left, type);
		p->right = new node;
		p->right->layer = p->layer + 1;
		create_tree(depth, p->right, type);
	}
	return p;
}

void tree::destroy_tree(node *p)
{
	  if(p!=NULL){
		  destroy_tree(p->left);	
		  destroy_tree(p->right);
		  delete p;
	  }
}

void tree::print(){
	print_tree(root_node, 0);
	printf("tree dpeth: %d\n", callayer(this->root_node, 0));
}

void tree::print_tree(node *p, int layer){
	if (p == NULL)
		return;
	print_tree(p->right, layer + 1);
	for (int i =0; i < layer + 1; i++)
		printf("\t");		
	fprintf(stdout, "%s\n", p->symbol);
	print_tree(p->left, layer + 1);
}

// Compare two tree
bool tree::compare(tree *t2){
	return cmp_tree(this->root_node, t2->root_node);
}

// Compare according to root_node
bool tree::cmp_tree(node *p1, node *p2){
	if(p1 == NULL && p2 == NULL)
		return true;
	else if(p1 != NULL && p2 != NULL){
		return( !strcmp(p1->symbol, p2->symbol) &&
				cmp_tree(p1->left, p2->left) &&
				cmp_tree(p1->right, p2->right));
	}
	else
		return false;
}

// Parse operators
double parse(char *ch, double a, double b){
	if (operatorNum(ch) == 0)
		return atof(ch);
	
	if (!strcmp(ch, "+"))
		return a+b;	
	else if (!strcmp(ch, "-"))
		return a-b;	
	else if (!strcmp(ch, "*"))
		return a*b;	
	else if (!strcmp(ch, "%")){
		if (b!=0)
			return a/b;	
		else 
			return 1;   
	}
	else if (!strcmp(ch, "sin"))			
		return sin(a);	
	else if (!strcmp(ch, "cos"))
		return cos(a);
	else if (!strcmp(ch, "exp"))	// May be too large?
		return exp(abs(a));
	else if (!strcmp(ch, "rlog")){
		return log(abs(a));
	}
	else if(!strcmp(ch, "sqrt"))
		return sqrt(abs(a));
	else if(!strcmp(ch, "pow")){
		return a*2.0;
	}
	else{ 
		puts("Error of operator.");
		return -1;
	}
}

// replace non-operator node (leaf) with real value(in char)
void tree::input_tree(node *p, char *value){
	if (p != NULL){
		if (!(strcmp(p->symbol, "+") && 
			strcmp(p->symbol, "-") && 
			strcmp(p->symbol, "*") && 
			strcmp(p->symbol, "%") && 
			strcmp(p->symbol, "sin") && 
			strcmp(p->symbol, "cos") && 
			strcmp(p->symbol, "exp") && 
			strcmp(p->symbol, "rlog")&&
			strcmp(p->symbol, "sqrt")&&
			strcmp(p->symbol, "pow")))
		{
			input_tree(p->left, value);
			input_tree(p->right, value);
		}
		else
			p->symbol = value;
	}
}

double tree::output(char *s){
	input_tree(this->root_node, s);
	double result = output_tree(this->root_node);
	input_tree(this->root_node, "x");	
	return result;
}

// Calculate tree output according to leaves
double tree::output_tree(node *p){
	if (operatorNum(p->symbol) == 0)
	{
		p->value = atof(p->symbol);
		return p->value;
	}

	else if (operatorNum(p->symbol) == 1){
		p->left->value = output_tree(p->left);
		p->value = parse(p->symbol, p->left->value, 0);
	}
	else
	{
		p->left->value = output_tree(p->left);
		p->right->value = output_tree(p->right);
		p->value = parse(p->symbol, p->left->value, p->right->value);
	}
	return p->value;
}

double tree::raw(){
	double err = 0;
	for(int i = 0; i < 101; i++){
		double e = output(x[i]) - atof(y[i]);
		err += e * e;
	}
	if(!(err<LDBL_MAX && err>-LDBL_MAX)){
	    err = exp(rand()%100+200);
	}
	input_tree(this->root_node, "x");
	return err;
}

int tree::nodenum(){
	return totlenode(root_node, 0);
}

int tree::totlenode(node *p, int num){
	if(p != NULL){
		num++;
		num += totlenode(p->left, 0);
		num += totlenode(p->right, 0);
		return num;
	}
	else 
		return 0;
}

void tree::copy(tree *t2){
	this->root_node = new node;
	copy_tree(root_node, t2->root_node);
}

void tree::copy_tree(node *p1, node *p2){
	if(p2 != NULL){
		memcpy(p1, p2, sizeof(node));
		if(operatorNum(p1->symbol) == 1){	
			p1->left = new node;
			copy_tree(p1->left, p2->left);
		}
		else if(operatorNum(p1->symbol) ==2){
			p1->left = new node;
			p1->right = new node;
			copy_tree(p1->left, p2->left);
			copy_tree(p1->right, p2->right);
		}
	}
}

tree *tree::crossover(tree *t2){
	tree *tmp1 = new tree[1];
	tree *tmp2 = new tree[1];
	
	tmp1->root_node = new node;
	tmp2->root_node = new node;
	
	copy_tree(tmp1->root_node, this->root_node);
	copy_tree(tmp2->root_node, t2->root_node);

	crossover_tree(tmp1->root_node, tmp2->root_node);
	
	int max, min;
	int tmp1_layer = callayer(tmp1->root_node, 0);
	int tmp2_layer = callayer(tmp2->root_node, 0);
	
	max = tmp1_layer > tmp2_layer ? tmp1_layer : tmp2_layer;
	
	while(max > MAX_DEPTH){

		copy_tree(tmp1->root_node, this->root_node);
		copy_tree(tmp2->root_node, t2->root_node);
		
		crossover_tree(tmp1->root_node, tmp2->root_node);
	
		tmp1_layer = callayer(tmp1->root_node, 0);
		tmp2_layer = callayer(tmp2->root_node, 0);
		max = tmp1_layer > tmp2_layer ? tmp1_layer : tmp2_layer;
	}


	copy_tree(this->root_node, tmp1->root_node);
	copy_tree(t2->root_node, tmp2->root_node);
	return t2;
}

void tree::crossover_tree(node *p1, node *p2){
	int cut_node_1 = rand() % totlenode(p1, 0) + 1;
	int cut_node_2 = rand() % totlenode(p2, 0) + 1;
	
	node *sub1 = new node[1];
	node *sub2 = new node[1];

	numbering(p1, 0);
	copy_tree(sub1, search(p1, cut_node_1));
	
	numbering(p2, 0);
	copy_tree(sub2, search(p2, cut_node_2));
	
	paste(p1, sub2, cut_node_1);
	paste(p2, sub1, cut_node_2);
}

int tree::callayer(node *p, int layer){
	if(p != NULL){
		++layer;
		int n = 0;
		int m = 0;
		n = callayer(p->left, layer);
		m = callayer(p->right, layer);
		layer = m>n ? m:n;
	}
	return layer;
}

int tree::numbering(node *p, int number){
	if (p != NULL){
		p->number = ++number;
		number = numbering(p->left, number);
		number = numbering(p->right, number);
	}
	return number;
}

tree::node * tree::search(node *p, int x){
	if( p != NULL){
		if (p->number == x)
			return p;
		else{
			node * t = search(p->left, x);
			if(t != NULL)
				return t;
			search(p->right, x);
		}
	}
	else
		return NULL;
}

bool tree::paste(node *p, node *sub, int x){
	if( p != NULL){
		if (p->number == x){
			destroy_tree(p->left);	
			destroy_tree(p->right);
			memcpy(p, sub, sizeof(node));
			return true;
		}
		else{
			if (paste(p->left, sub, x))
				return true;
			else if(paste(p->right, sub, x))
				return true;
			else
				return false;
		}
	}
	else
		return false;
}

void tree::mutate(){
	
	tree *tmp = new tree[1];
	tmp->root_node = new node;
	copy_tree(tmp->root_node, this->root_node);
	
	mutate_tree(tmp->root_node);
	
	while(callayer(tmp->root_node, 0) > MAX_DEPTH){
		copy_tree(tmp->root_node, this->root_node);
		mutate_tree(tmp->root_node);
	}
	
	copy_tree(this->root_node, tmp->root_node);
}

void tree::mutate_tree(node *p){
	int cut_node = rand() % totlenode(p, 0) + 1;

	node *s = new node[1];
	s->layer = 0;
	// added subtree less than 3 depth
	int s_depth = rand()%3+1;
	bool type = rand()%2;
	
	if(s_depth == 1 && cut_node != 1){
		s->symbol = pick(0);
		s->left = NULL;
		s->right = NULL;
	}
	else{
		s_depth = rand()%3+5;
		create_tree(s_depth, s, type); 
	}
	
	numbering(p, 0);
	paste(p, s, cut_node);

}

char *tree::simplify(){
	char str[20000] = {0};
	printf("simplified function: %s\n", simplify_tree(this->root_node, str));
}

char *tree::simplify_tree(node *p, char *str){
	if(p!=NULL){
		if(operatorNum(p->symbol)==0)
			strcpy(str, "x");
		else{
			if(operatorNum(p->symbol)==2){
				char s[20000]="";
				//char *s= new char[20000];
				strcat(str, "(");
				strcat(str, simplify_tree(p->left, s));
				strcat(str, p->symbol);
				strcpy(s, "");
				strcat(str, simplify_tree(p->right, s));
				strcat(str, ")");
			}
			if(operatorNum(p->symbol)==1){
				char s[20000] = "";
				//char *s= new char[20000];
				strcat(str, p->symbol);
				strcat(str, "(");
				strcat(str, simplify_tree(p->left, s));
				strcat(str, ")");
			}
		}
	return str;
	}
	else
		return "";
}

void tree::write(){
	FILE *file;
	file = fopen("best.txt","w+");
	write_tree(root_node, file);
	fclose(file);
}
void tree::write_tree(node *p, FILE *file){
	if (!p){
		fprintf(file, "%s","#\n");
	} 
	else{
		fprintf(file, "%s\n", p->symbol);
		write_tree(p->left, file);
		write_tree(p->right, file);
	}
}

void tree::read(char *s){
	FILE *file;
	file = fopen("best.txt", "r");
	if(file == NULL)
		perror("Error opening file.");
	else{
		this->root_node = new node;
		read_tree(this->root_node, file);
		input_tree(this->root_node, s);
		double result = output_tree(root_node);
		printf("\t\t>>>>>>>>>> Result:\t%f\n", result);
	fclose(file);
	}
}

tree::node * tree::read_tree(node *p, FILE *file) {
	readline[10]= NULL;
	  if (fgets(readline, sizeof(readline), file)!=NULL){
		readline[strlen(readline)-1]='\0';	
		if(!strcmp(readline, "#")){
			return NULL;	
		}
		else{
			if(p==NULL)
				p = new node;
			char tmp[10];
			p->symbol = new char[10];
			memcpy(p->symbol, readline, sizeof(readline));
			if(operatorNum(p->symbol) == 0){
				p->left = read_tree(p->left, file);
				p->right = read_tree(p->right, file);
			}
			else if(operatorNum(p->symbol) == 1){
				p->left = new node;
				p->left = read_tree(p->left, file);
				p->right = read_tree(p->right, file);			
			}
			else if(operatorNum(p->symbol) == 2){
				p->left = new node;
				p->right = new node;
				p->left = read_tree(p->left, file);
				p->right = read_tree(p->right, file);
			}
			else{
				puts("Error in read_tree");
				exit(1);
			}
			return p;
		}
	}
}

// Generate random operators. 0:x, 1:F[]
char * pick(int flag){
	char *result;
	result = (char *)calloc(100, sizeof(char));
	if (flag == 0){
		strcpy(result, "x");
	}
	else 
		strcpy(result, F[rand()%6]);
	return result;
}

int operatorNum (char *ch){
	int opNum = 0;
	for(int i = 0; i < 6; i++){
			if (!strcmp(ch, F[i])){
				if (i<4)
					opNum = 2;
				else
					opNum = 1;
				return opNum;
			}
			else
				opNum = 0;
		}
	return opNum;
}
