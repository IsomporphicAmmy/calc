#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

enum token_type
{
	NUMBER,
	BRACKET,
	OPERAND
};

typedef struct Tree
{
	int branches_count;
	char operand;
	int value;
	struct Tree **branches;
} Tree;

typedef struct Token
{
	enum token_type type;
	char value;
	int num_value;
} Token;

typedef struct TokenArray
{
	int size;
	Token *arr_ptr; 
} TokenArray;

void add_token(TokenArray *input_array, Token input_token)
{
	if (input_array->size == 0)
	{
		input_array->size++;
		input_array->arr_ptr = malloc(sizeof(Token));
		input_array->arr_ptr[0] = input_token;
	}
	else
	{
		input_array->size++;
		int s = input_array->size;
		input_array->arr_ptr = realloc(input_array->arr_ptr, sizeof(Token) * s);
		input_array->arr_ptr[s - 1] = input_token;
	}
}

void dig2num(TokenArray *input)
{
	TokenArray result = {0, NULL};
	int i = 0;
	while (i < input->size)
	{
		Token temp;
		if (input->arr_ptr[i].type == BRACKET)
		{
			temp.type = BRACKET;
			temp.value = input->arr_ptr[i].value;
			temp.num_value = 0;
			add_token(&result, temp);
			i++;
		}
		else if (input->arr_ptr[i].type == OPERAND)
		{
			temp.type = OPERAND;
			temp.value = input->arr_ptr[i].value;
			temp.num_value = 0;
			add_token(&result, temp);
			i++;
		}
		else
		{
			temp.type = NUMBER;
			int temp_int = 0;
			int magnitude = 0;

			while ((i + magnitude + 1) < input->size)
			{
				if(input->arr_ptr[i + magnitude + 1].type != NUMBER)
				{
					break;
				}
				magnitude++;
			}
			for (int j = magnitude; j >= 0; j--)
			{
				temp_int += input->arr_ptr[i].num_value * pow(10, j);
				i++;
			}

			temp.num_value = temp_int;
			temp.value = '\0';
			add_token(&result, temp);
		}
	}
	free(input->arr_ptr);
	input->size = result.size;
	input->arr_ptr = realloc(result.arr_ptr, sizeof(Token) * result.size);
}

TokenArray tokenizer(char *input)
{
	TokenArray result = {0, NULL}; 
	for(int i = 0; input[i] != '\0'; i++) 
	{
		Token temp;
		if (input[i] >= '0' && input[i] <= '9')
		{
			temp.type = NUMBER;
			temp.num_value = input[i] - '0';
			add_token(&result, temp);
		}
		else if (input[i] == '(' || input[i] == ')')
		{
			temp.type = BRACKET;
			temp.value = input[i];
			add_token(&result, temp);
		}
		else if (input[i] == '+' ||
			 input[i] == '-' ||
			 input[i] == '*' ||
			 input[i] == '/')
		{
			temp.type = OPERAND;
			temp.value = input[i];
			add_token(&result, temp);
		}
		else if(input[i] != ' ' && input[i] != '\n' && input[i] != '\0') 
		{
			printf("%d\n", input[i]);
			printf("Input error\n");
			exit(1);
		}
	}
	dig2num(&result);
	return result;
}

Tree parser(const TokenArray *input_array)
{
	Tree result = {0, 0, 0, NULL};
	
	for (int i = 0; i < input_array->size; i++)
	{
		if(input_array->arr_ptr[i].value == '*' || input_array->arr_ptr[i].value == '/')
		{
			result.branches_count++;
		}
	}
	result.branches = malloc(sizeof(Tree*) * result.branches_count);
	return result;
}

int main()
{
	char buffer[64];
	fgets(buffer, 64, stdin);
	TokenArray tokens = tokenizer(buffer);
	for(int i = 0; i < tokens.size; i++)
	{
		if (tokens.arr_ptr[i].type == NUMBER)
		{
			printf("Token type: NUMBER\nnum_value: %d\nvalue: %c\n", tokens.arr_ptr[i].num_value, tokens.arr_ptr[i].value);
		}
		else if (tokens.arr_ptr[i].type == BRACKET)
		{
			printf("Token type: BRACKET\nnum_value: %d\nvalue: %c\n", tokens.arr_ptr[i].num_value, tokens.arr_ptr[i].value);
		}
		else if (tokens.arr_ptr[i].type == OPERAND)
		{
			printf("Token type: OPERAND\nnum_value: %d\nvalue: %c\n", tokens.arr_ptr[i].num_value, tokens.arr_ptr[i].value);
		}
	}
	free(tokens.arr_ptr);
	Tree operation_tree = parser(&tokens);
	printf("%d\n", operation_tree.branches_count);
	free(operation_tree.branches);
	return 0;
}
