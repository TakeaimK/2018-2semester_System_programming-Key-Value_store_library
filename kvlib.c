/*
----------------------------------------------------------------------
	System programming Team Project : make key-value store library
	
							201414825 �̹���
							201511033 �ּ���
							201615407 �̰��� 

----------------------------------------------------------------------
*/
//�ʿ��� ��� include 
#include<stdio.h>
#include<stdlib.h>
#include<strings.h>

#define SUCCESS   0			//������ 0 return 
#define FAULT   1			//���н� 1 return 

#define TSIZE    357913      //100000 x 3�� ����� �Ҽ�(������ 100000������ �� 30%�� ���� �߰�)
#define KEY_MAX_SIZE 4097   // KEY MAX SIZE IS 4096 + '\0'
#define VALUE_MAX_SIZE 513   // VALUE MAX SIZE IS 512 + '\0'

typedef struct TreeNode            // ����Ž��Ʈ���� ��� �� ��� ����ü 
{
   char key[KEY_MAX_SIZE];       // file�κ��� �޾ƿ� key ���� ����Ǳ� ���� ����
   char value[VALUE_MAX_SIZE];      // file�κ��� �޾ƿ� data ���� ����Ǳ� ���� ����

   struct TreeNode* left;         // TreeNode ���� �ڽĳ��
   struct TreeNode* right;         // TreeNode ������ �ڽ� ���
   
}NODE;

typedef struct hashTable         // �ؽ����̺� ����ü 
{
   NODE* tableIndex[TSIZE];      // ������� 10007 
   
}TABLE;

TABLE table;		//���� ���̺� ���� 


int hashFunction(char *key)         // �ؽ� �Լ� 
{
   unsigned int hash = 0; 		//���̺��� ������ �����Ƿ� unsigned ���� 
   int tmn = *key;				//ù key�� ��� 
   while(*key)                  // key�� ù°�ڸ����� NULL�� ���ö����� �ݺ� 
   {
      hash = hash + *key;       // �� �ڸ��� ���ڸ� int������ �����ش�. 
      *key++;                  // ���� ����Ʈ�� ���ڷ� �Ѿ 
   }
   hash*=71;					//�Ҽ� 71�� ���� 
   if((tmn - 97) < 47){			//ù key�� ���� ���� ���̺� ��ġ �̼� ���� 
   	hash+=tmn;
   }
   else{
   	hash+=((tmn - 97) - 61);
   }
   return hash % TSIZE;			//���̺� ���� ������ ������ ���� 
}

int insertNode(NODE **root, char *key, char *value)
{
   NODE *parent, *child;                            //�θ���, �ڽĳ��
   NODE *newNode; // = (NODE*)malloc(sizeof(NODE));         //���ο���

   child = *root;                                 // �ڽĳ�带 root�� ����( �ؽ� ���̺�)
   parent = NULL;                                  // �θ� ����ش�.
   
   //Ž���� ���� ����
   while(child != NULL)                            // NULL�� �ɶ����� �ݺ�  
   {  
      if(strcmp(key, child->key)==0)
      {
         strncpy(child->value, value, strlen(value));   //key���� ������ value ����� 
         return SUCCESS;                           //���� key���� ����� key ���� ���� ��� insert ���� 
      } 
      parent=child;                              // �θ� ���� ��尪�� �־��ش�. �θ�� �ڽ��� ��ũ���ֱ� ���ؼ� �θ� �� �ʿ䰡 ���� 
      if(strcmp(key, child->key)<0) child = child->left;   // key������ ���� ����� key���� ���� ��� �����ڽ�Ʈ���� �̵�
      else child = child->right;                      // key������ ���� ����� key���� Ŭ ��� ���������� �̵� 
   }
   
   //item�� Ʈ���ȿ� �����Ƿ� ���԰���
   newNode = (NODE*)malloc(sizeof(NODE));
   
   if(newNode == NULL)                              // �����Ҵ��� �ȵɰ�� 
   {
      printf("Memory falut in insertNode\n");
      return FAULT;
   }
   //������ ����
   strcpy(newNode->key, key);                         // ��忡 key�� ���� 
   strcpy(newNode->value, value);                      // ��忡 value�� ����
   newNode-> left = NULL;                          // ���� �� 
   newNode-> right = NULL;                         // �������� ����ش� 
   //�θ���� ��ũ����
   if(parent != NULL){                               // �θ� NULL�� �ƴ� ��� 
      if(strcmp(key, parent->key)<0) {                  // key������ ���ؼ� ������� 
         parent->left = newNode; 
      }                  // �θ��� ���ʿ� ��带 �ִ´� 
      else{                                     // Ŭ��� 
         parent->right = newNode;
      }
   }                   // �����ʿ� �ִ´� 
   else
      *root = newNode;                            // �θ� ���ٸ� root�� �־��ְ� ���� 
      
   return SUCCESS;
}

int kvget(char *key, char *buf)
{
   int index = hashFunction(key);
   
   NODE* searchKeyValue; // �Ű������� �Է¹��� key�� ����� ����Ž��Ʈ�� ��带 �˻��ϱ� ���� BinSearchTreeNode ������ �� ����ü ����
   searchKeyValue = table.tableIndex[index]; // pBinSearchTreeNode�� ã���� �ϴ� key�� ù ��° ���ڷ� ������ ����Ž��Ʈ���� rootNode�� ����
   
   while(searchKeyValue != NULL){ // ���� ���� key�� �� �ݺ� 
      if(strcmp(key, searchKeyValue->key)<0)
      { 
         searchKeyValue = searchKeyValue->left; // ��������
      }
      else if(strcmp(key, searchKeyValue->key)==0)
      {
         strncpy(buf, searchKeyValue->value, strlen(searchKeyValue->value)+1); // buf�� ���� 
         return SUCCESS;
      }
      else if(strcmp(key, searchKeyValue->key)>0)
      { 
         searchKeyValue = searchKeyValue->right;// ����������
      }
   }
   
   strcpy(buf, ""); // key���� �����Ƿ� buf���� NULL�� �ʱ�ȭ
   return FAULT;
}

int kvput(char *key, char *value)
{
   int index = hashFunction(key);	// index = key�� �ؽ�
   
   insertNode(&(table.tableIndex[index]), key, value);   // key���� value���� ����  insert �����ش�. 
   return SUCCESS;
}

// �־��� key�� �̿��Ͽ� key�� value�� �����Ѵ�. 
// �������� �ʴ� key�� �����Ϸ��ϸ� �׳� 0�� �����Ѵ�.
int kvdel(char *key)
{
    int index = hashFunction(key);      // index = key �� �ؽ�
    NODE *parentNode, *childNode, *currentNode, *successorNode, *predecessorNode;
   parentNode = NULL;                                           // �θ���� ����ְ� 
   currentNode = table.tableIndex[index];                           // index�� ù�κк��� search 
   
   while(currentNode != NULL && strcmp(currentNode->key, key) != 0)       // currentNode�� ������� �ʰ�, ����key���� ����� key ���� �ٸ����  
   {
      parentNode = currentNode;                                  // currentNode�� �θ� �ְ� 
      if(strcmp(key, currentNode->key)<0) {
         currentNode = currentNode->left;
      }// Ű���� �����尪�� ���Ͽ� �̵� 
      else {
         currentNode = currentNode->right;
      }
   }
   
   if(currentNode == NULL) return FAULT;                           //Ž��Ʈ���� ���� Ű
   
   //�ܸ�����ΰ��
   if((currentNode->left == NULL)&&(currentNode->right == NULL)) 
   {
      if(parentNode != NULL)                               // �θ��尡 ������ 
      {
         if(parentNode->left == currentNode)                // �����ڽ��� currentNode�� ��� 
            parentNode->left = NULL;                            //������ ����ش�. 
         else                                              // ������ �ڽ��� ��� 
            parentNode->right = NULL;                            // �������� ����ش�. 
      }
      else {                                              //�θ��尡 ������ ��Ʈ
         table.tableIndex[index] = NULL;
      }
      return SUCCESS;
   }
   
   //�ΰ����ڽ��� ������ ���
   else if((currentNode->left != NULL)&&(currentNode->right != NULL))
   {
      predecessorNode = currentNode;      					 //������ ����Ʈ���ּ�
      successorNode = currentNode->right;   				 // �ļ��ڸ� ã�´� 
      while(successorNode->left != NULL)					//���� �ڽ��� NULL�� ������ 
      {
         predecessorNode = successorNode;      				//�־� �ְ� 
         successorNode = successorNode->left;   			 // �����ڽ����� �̵� 
      }
      strcpy(currentNode->key, successorNode->key); 			// ��忡 key�� ���� 
      strcpy(currentNode->value, successorNode->value); 		// ��忡 value�� ����
      
      if(predecessorNode->left == successorNode)        		 //�ļ����� �θ�� 
         predecessorNode->left = successorNode->right;    	  //�ڽ��� ����
      else
         predecessorNode->right = successorNode->right;  
      return SUCCESS;
   }
   
   //�ϳ��� �ڽĸ� �ִ°��
   else if(((currentNode->left != NULL)&&(currentNode->right == NULL)) || ((currentNode->left == NULL)&&(currentNode->right != NULL)))
   {
      if(currentNode->left != NULL) childNode = currentNode->left;
      else childNode = currentNode->right;
      
      if(parentNode != NULL) 						// �θ� ������� ���� ��� 
      {    											//�θ��带 �ڽĳ��� ����
         if(parentNode->left == currentNode)
            parentNode->left = childNode; 
         else                  
            parentNode->right = childNode;   
      }
      else
         table.tableIndex[index] = childNode; 		// �θ��尡 ������ ��Ʈ 
         
      return SUCCESS;
   }
   else{	//�̵� ���� �ƴ� ��� ���� ó�� 
      return FAULT;
   }
}

int kvopen()
{
   int index;												//�ؽ��Լ� ������� ������ ���� 
   char key[KEY_MAX_SIZE];									//key�� �ӽ÷� ���� �迭 
   char value[VALUE_MAX_SIZE];								//value�� �ӽ÷� ���� �迭 
   NODE* tempNode;												//����ü read �� �־��� �ӽ� node 
   FILE *keyValueText = fopen("keyValue.bin", "ab+");             // keyValue ������ ���̳ʸ� ���� ���� 
      
   if(feof(keyValueText)==FAULT)	//���� ������ ���� 
   {
      printf("keyValue File is ERROR.\n");
      return FAULT;
   }
   
   if(keyValueText != NULL)     //keyValueText�� ���� ������ ������ key-value�� �ҷ����� ���Ͽ� 
   {
      while(1)         //keyValueText���� ������ read
      {
         fread(&tempNode, sizeof(*tempNode), 1, keyValueText);
         if(feof(keyValueText)) break;								//������ ���̶�� ���� 
         
         index = hashFunction(key);                    		     // key�� �ؽ� 
         insertNode(&(table.tableIndex[index]), key, value);    // key value�� ���� insert �����ش�.
      }   
   }
   fclose(keyValueText);			//���� �ݱ� 
    return SUCCESS;
}

// Ʈ���� ��ȸ�ϸ� ���Ͽ� ����.
void storeTreeToFile_RECUR(NODE * root, FILE * KeyValueText)
{    
   
   if(root != NULL)		//��Ʈ�� ���ư� ������ ��ȸ. ���� ��ȸ�� ��� 
   {
      if(root->left != NULL)
         storeTreeToFile_RECUR(root->left, KeyValueText);		//���� �ڽ����� ��� 
      fwrite(&root, sizeof(*root), 1, KeyValueText);
      if(root->right != NULL)
         storeTreeToFile_RECUR(root->right, KeyValueText);		//������ �ڽ����� ��� 
   }
}

int kvclose()
{
   int index;
   FILE *keyValueText = fopen("keyValue.bin", "wb+");		//������� ���� ���� 
   NODE* currentNode;      // ���� ��忡  root ���� 

   for(index=0; index<TSIZE; index++)	// ���̺� ��ü ��ȸ 
   {
      if(table.tableIndex[index]!=NULL)		//���̺��� ��Ʈ��尡 �ִ� ��츸 ���� 
      {
         currentNode = table.tableIndex[index];		//�ֻ��� ��带 ���� 
         storeTreeToFile_RECUR(currentNode, keyValueText);		//����Լ� ȣ�� 
      
      }
   }
   fclose(keyValueText);			//���� �ݱ� 
   return SUCCESS;
}
