#include <iostream>
#include <cstring>
#include<stdio.h>
#include<ctype.h>
#define MAX 50

char postfix[1000];
char stack[100];
int top = -1;

int stackx[MAX];
int stackxTop=-1;

int pow(int base, int exponent)
{
  int x = 1;
  for(int j = 0; j < exponent; j++)
    x *= base;
  return x;
}

void pushstackx(int sample)
{
   stackxTop++;
   stackx[stackxTop]=sample;
}

void evaluate(char c)
{
   int a, b, ans;
   a = stackx[stackxTop];
   stackx[stackxTop--] = '\0';
   b = stackx[stackxTop];
   stackx[stackxTop--] = '\0';
   switch(c)
   {
      case '+':
        ans = b + a;
        break;
      case '-':
        ans = b - a;
        break;
      case '*':
        ans = b * a;
        break;
      case '/':
        ans = b / a;
        break;
      case '%':
        ans = b % a;
        break;
      case '^':
        ans = pow(b,a);
        break;
      default:
          ans = 0;
   }
   stackxTop++;
   stackx[stackxTop] = ans;
}

int calc()
{
   int i,l;
   l = strlen(postfix);
   int sample = 0, flag = 0;
   for(i = 0; i < l; i++)
   {
      if(postfix[i]>='0' && postfix[i]<='9')
      {
        sample = 10*sample + (int(postfix[i]-'0'));
        flag = 1;
      }
      if(postfix[i] == ' ' && flag) {
        pushstackx(sample);
        flag = 0;
        sample = 0;
      }
      if(postfix[i]=='+' || postfix[i]=='-' || postfix[i]=='*' || postfix[i]=='/' || postfix[i]=='^' || postfix[i]=='%')
          evaluate(postfix[i]);
   }
   return stackx[stackxTop];
}

void push(char x)
{
    stack[++top] = x;
}

char pop()
{
    if(top == -1)
        return -1;
    else
        return stack[top--];
}

int priority(char x)
{
  if(x == '(')
    return 0;
  if(x == '+' || x == '-')
    return 1;
  if(x == '%' || x == '*' || x == '/')
    return 2;
  if(x == '^')
    return 3;
  return 0;
}

void fillpostfix(char* exp)
{
    char *e, x;
    e = exp;
    int i = 0;
    int sample = 0;
    int flag = 0;
    while(*e != '\0')
    {
      if(isalnum(*e)){
        postfix[i++] = *e;
        flag = 1;
      }
      else if(*e == '(')
      {
          push(*e);
          if(flag){
            printf("ERROR: no operator between number and parentheses\n");
            flag = 0;
          }
      }
      else if(*e == ')')
      {
        if (flag) {
          postfix[i++] = ' ';
          flag = 0;
        }
        while((x = pop()) != '('){
          postfix[i++] = x;
          postfix[i++] = ' ';
        }
      }
      else
      {
        if (flag) {
          postfix[i++] = ' ';
          flag = 0;
        }
          while(priority(stack[top]) >= priority(*e)){
              postfix[i++] = pop();
              postfix[i++] = ' ';
            }
          push(*e);
      }
      e++;
    }
    while(top != -1){
      char temp2 = pop();
      if(isalnum(temp2))
      {
        postfix[i++] = temp2;
        flag = 1;
      }
      else
      {
        if (flag)
        {
          postfix[i++] = ' ';
          flag = 0;
        }
        postfix[i++] = temp2;
        postfix[i++] = ' ';
      }
    }
}

int main()
{
  char exp[100];
  printf("Enter the expression : ");
  scanf("%s",exp);
  fillpostfix(exp);
  int ans = calc();
  printf("%d\n", ans);
}
