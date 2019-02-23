#include<iostream>
#include<conio.h>
#include<ctype.h>
#include<string.h>
#include<math.h>
#include<fstream>
#include<stdio.h>
#include<iomanip>
#include<stdlib.h>
using namespace std;
struct person //defining structure to store data for individual user in file
{
 char name[100];
 char userid[30];
 char password[30];
 char words[100][100];
 char meanings[100][500];
 int n;
};
struct universal //defining structure to store words for common dictionary
{
 char word[100];
 char meaning[100];
};
struct tree //defining structure to build tree 
{
 char word[100], meaning[100];
 struct tree *left;
 struct tree *right;
};
typedef struct tree * bstnode;
bstnode record[26]; //26 trees accessed via hashing dependent on the first character of the word
bstnode Insert(bstnode t, char w[], char m[]) //to insert the word and meaning in the tree with root d
{
 if(t==NULL)
 {
 t=(bstnode)malloc(sizeof(struct tree *));;
 strcpy(t->word,w);
 strcpy(t->meaning,m);
 t->right=NULL;
 t->left=NULL;
 }
 else if(strcmp(w,t->word)<0)
 t->left=Insert(t->left,w,m);
 else if(strcmp(w,t->word)>0)
 t->right=Insert(t->right,w,m);
 return t;
}
void build(char word[100], char meaning[500]) //to decide the appropriate root for the word and meaning to be entered into
{
 int hf; 
 char ch;
 ch=word[0]; //creating a hash table with reference to the first character
 hf=int(ch)-97; //all words must be in small letters
 record[hf]=Insert(record[hf], word, meaning);
}
void temp() //to extract a set of word and meaning from the file and pass the set to build function
{
 universal v;
 ifstream ifs("universal.dat", ios::binary | ios::in);
 while(!ifs.eof())
 {
 ifs.read((char *)&v, sizeof(v));
 build(v.word,v.meaning);
 }
}
int add_words_personal() //to add words to the file maintained for personal use
{
 person s;
 char userid[30], password[30], ch;
 int i,n,flag=0;
 ofstream ofs("temp.dat", ios::out | ios::app | ios::binary);
 ifstream ifs("personal.dat", ios::in | ios::binary); //to match password of user id which is being modified
 system("cls"); //to clear screen of all previous outputs
 cout<<"\nEnter user ID: ";
 cin>>userid;
 while(!ifs.eof())
 { 
 ifs.read((char *)&s, sizeof(s));
 if(strcmp(userid,s.userid)!=0)
 {
 ofs.write((char *)&s, sizeof(s));
 }
 else
 {
 flag=1;
 cout<<"Enter password to validate: ";
 for(i=0;i<30;i++) //loop to facilitate hiding of characters of password by '*'
 {
 ch=getch(); //takes input of character but doesn't show character on screen
 if((int)ch==13) //to check if enter key is pressed
 break;
 password[i]=ch; //if enter key isn't pressed, store character entered as character of password
 cout<<"*"; //display '8' for every character entered unless enter key is detected
 }
 password[i]='\0';
 if(strcmp(password,s.password)==0)
 {
 cout<<"\n\nAction validated! \nPress enter key to continue";
 getch(); //to ensure program waits for user to press a key before continuing to next step
 system("cls"); //to clear screen of all previous outputs
 cout<<"\nEnter number of words to be added to user's private dictionary: ";
 cin>>n;
 for(i=s.n;i<s.n+n;i++)
 {
 cout<<"\nEnter word "<<i+1<<": ";
 cin>>s.words[i]; 
 cout<<"Enter meaning of word: ";
 cin>>s.meanings[i];
 }
 s.n+=n;
 ofs.write((char *)&s, sizeof(s));
 }
 else
 {
 system("cls"); //to clear screen of all previous outputs
 cout<<"\nWrong password!\nRedirecting to main menu\nPress enter key to continue";
 getch(); //to ensure program waits for user to press a key before continuing to next step
 return 1;
 }
 }
 }
 ifs.close();
 ofs.close();
 remove("personal.dat");
 rename("temp.dat","personal.dat");
 if(flag==0)
 {
 system("cls"); //to clear screen of all previous outputs
 cout<<"\nNo such user ID found!\nReturning to main menu!\nPress enter key to continue";
 getch(); //to ensure program waits for user to press a key before continuing to next step
 return 0;
 }
 else
 { 
 system("cls"); //to clear screen of all previous outputs
 cout<<"\nWords saved in user's record\nReturning to main menu!\nPress enter key to continue";
 getch(); //to ensure program waits for user to press a key before continuing to next step
 return 1;
 }
}
void add_words_universal() //to add words the file maintained for common words
{
 universal u;
 int choice=1;
 ofstream ofs("universal.dat", ios::out | ios::app | ios::binary);
 while(choice==1)
 {
 system("cls"); //to clear screen of all previous outputs
 cout<<"Enter words for universal dictionary\n "<<endl;
 cout<<"Enter word: ";
 cin>>u.word;
 cout<<"Enter meaning for word: ";
 cin>>u.meaning;
 ofs.write((char *)&u, sizeof(u));
 cout<<"\n\nEnter 1 to add more words";
 cout<<"\nEnter 2 to return to main menu";
 cout<<"\n\nEnter your choice: ";
 cin>>choice;
 }
 ofs.close();
}
void add_words_menu() //to determine which file the user wants to add words to 
{
 int choice,flag;
 do
 {
 system("cls"); //to clear screen of all previous outputs
 cout<<"\n\tAdd Words Menu\n";
 cout<<"\n1. Add words to personal dictionary";
 cout<<"\n2. Add words to universal dictionary";
 cout<<"\n0. Return to main menu";
 cout<<"\n\nEnter your choice: ";
 cin>>choice;
 switch(choice)
 {
 case 1:
 flag=add_words_personal();
 if(flag==1)
 {
 return;
 }
 break;
 case 2:
 add_words_universal();
 temp();
 return;
 case 0:
 system("cls"); //to clear screen of all previous outputs
 cout<<"Returning to main menu!\nPress enter key to continue";
 getch();
 return;
 default: 
 system("cls"); //to clear screen of all previous outputs
 cout<<"\nWrong input!\nTry Again\nRedirecting to add words menu!\nPress enter key to continue";
 getch();
 }
 }while(choice!=0);
}
void create_user() //to create a new user Id
{
 system("cls"); //to clear screen of all previous outputs
 int i,flag=1;
 person s;
 char ch, name[100], userid[30];
 cout<<"Warning!";
 cout<<"\nRules of entering name, user-name and password:";
 cout<<"\n1. Name can have a maximum of 100 characters";
 cout<<"\n2. User-name and password can have a maximum of 30 characters each";
 cout<<"\n3. Name, user-name and password cannot have a space in the middle";
 cout<<"\n4. For your own safety, choose a password which is a mix of alphabets, special characters and numbers";
 cout<<"\n5. Each user is allowed to save a maximum of 100 words";
 cout<<"\n6. The meaning associated with each can have a maximum of 500 characters each";
 cout<<"\n\nPress enter key to continue";
 getch(); //to ensure program waits for user to press a key before continuing to next step
 system("cls"); //to clear screen of all previous outputs
 ofstream ofs("personal.dat", ios::out | ios::app | ios::binary);
 ifstream ifs("personal.dat", ios::in | ios::binary);
 cout<<"\nEnter name of user: ";
 cin>>name; 
 s.n=0; //initially 0 words are stored in structure
 do //loop to make sure repetition of user-id doesn't take place
 {
 flag=1;
 cout<<"Enter desired user ID: ";
 cin>>userid;
 while(!ifs.eof())
 {
 ifs.read((char *)&s, sizeof(s));
 if(strcmp(userid,s.userid)==0)
 {
 flag=0;
 cout<<"\nThis user ID is taken!\nTry again...\n\n";
 break;
 }
 }
 }while(flag==0);
 ifs.close();
 cout<<"Enter desired password: ";
 for(i=0;i<30;i++) //loop to facilitate hiding of characters of password by '*'
 {
 ch=getch(); //takes input of character but doesn't show character on screen
 if((int)ch==13) //to check if enter key is pressed
 break;
 s.password[i]=ch; //if enter key isn't pressed, store character entered as character of password
 cout<<"*"; //display '8' for every character entered unless enter key is detected
 }
 s.password[i]='\0';
 cout<<"\nEnter number of words you want to store initially: ";
 cin>>s.n; 
 for(i=0;i<s.n;i++) //to store initial number of words
 {
 cout<<"\nEnter word "<<i+1<<": ";
 cin>>s.words[i];system("cls"); //to clear screen of all previous outputs
 cout<<"Enter meaning of word: ";
 cin>>s.meanings[i];
 }
 strcpy(s.name,name);
 strcpy(s.userid,userid);
 ofs.write((char *)&s, sizeof(s)); //write user data in file for long term record
 ofs.close();
 cout<<"\n\nUser created in record!\nPress enter to return to main menu";
 getch(); //to ensure program waits for user to press a key before continuing to next step
}
void delete_user() //to remove details of user from record
{
 person s;
 ifstream ifs("personal.dat", ios::in | ios::binary);
 ofstream ofs("temp.dat", ios::out | ios::binary);
 char user[30], ch, password[30];
 int choice,i,flag=0;
 system("cls"); //to clear screen of all previous outputs
 cout<<"\nEnter user ID to be deleted: ";
 cin>>user;
 while(!ifs.eof())
 {
 ifs.read((char *)&s, sizeof(s));
 if(strcmp(user,s.userid)==0)
 { 
 flag=1; //to indicate that entered user-name exists in records
 cout<<"\nUser ID found!\nPress enter key to continue";
 getch(); //to ensure program waits for user to press a key before continuing to next step
 system("cls"); //to clear screen of all previous outputs
 cout<<"\nPersons records being deleted is: ";
 cout<<"\nName: "<<s.name;
 cout<<"\nUser ID: "<<s.userid;
 cout<<"\nEnter password to validate: ";
 for(i=0;i<30;i++) //loop to facilitate hiding of characters of password by '*'
 {
 ch=getch(); //takes input of character but doesn't show character on screen
 if((int)ch==13) //to check if enter key is pressed
 break;
 password[i]=ch; //if enter key isn't pressed, store character entered as character of password
 cout<<"*"; //display '8' for every character entered unless enter key is detected
 }
 password[i]='\0';
 if(strcmp(password,s.password)==0)
 {
 cout<<"\n\nAction validated! \nPress enter key to continue";
 getch(); //to ensure program waits for user to press a key before continuing to next step
 do
 {
 system("cls"); //to clear screen of all previous outputs
 cout<<"\nEnter 1 to return to main menu and cancel deletion";
 cout<<"\nEnter 2 to confirm deletion of record";
 cout<<"\n\nEnter your choice: ";
 cin>>choice; 
 switch(choice)
 {
 case 1:
 system("cls"); //to clear screen of all previous outputs
 ofs.write((char *)&s, sizeof(s));
 cout<<"\n\nRecord not deleted!\nReturning to main menu\nPress enter key to continue";
 getch(); //to ensure program waits for user to press a key before continuing to next step
 break;
 case 2:
 cout<<"\nNode being deleted\nPress enter key to continue";
 getch(); //to ensure program waits for user to press a key before continuing to next step
 choice=1;
 break;
 default:
 {
 system("cls"); //to clear screen of all previous outputs
 cout<<"Wrong input!\nTry Again...";
 getch(); //to ensure program waits for user to press a key before continuing to next step
 }
 }
 }while(choice!=1);
 break;
 }
 else
 {
 system("cls"); //to clear scresystem("cls"); //to clear screen of all previous outputsen of all previous outputs 
 cout<<"\nWrong password!\nRedirecting to main menu\nPress enter key to continue";
 getch(); //to ensure program waits for user to press a key before continuing to next step
 return;
 }
 }
 else
 {
 ofs.write((char *)&s, sizeof(s));
 }
 }
 if(flag==0)
 {
 cout<<"\nNo such user ID found \nRedirecting to main menu!";
 getch(); //to ensure program waits for user to press a key before continuing to next step
 return;
 }
 ifs.close();
 ofs.close();
 remove("personal.dat");
 rename("temp.dat","personal.dat");
}
int exit_menu() //to give the user a confirmation if he wants to delete
{
 int x;
 system("cls"); //to clear screen of all previous outputs
 cout<<"\n1. Confirm exit";
 cout<<"\n2. Return to program"; 
 cout<<"\n\nEnter your choice: ";
 cin>>x;
 if(x==1)
 {
 system("cls"); //to clear screen of all previous outputs
 cout<<"\nExiting from program!";
 getch(); //to ensure program waits for user to press a key before continuing to next step
 exit(0);
 }
 else if(x==2)
 {
 system("cls"); //to clear screen of all previous outputs
 cout<<"\nPress enter key to return to main menu!";
 getch(); //to ensure program waits for user to press a key before continuing to next step
 }
 return 1;
}
void search_word() //search for given word in the dictionary
{
 int hf,flag=0;
 char ch, word[100];
 bstnode n;
 system("cls"); //to clear screen of all previous outputs
 cout<<"\nEnter word to search for: ";
 cin>>word;
 ch=word[0];
 hf=int(ch)-97;
 n=record[hf];
 //cout<<endl<<ch<<endl<<hf<<endl; 
 while(n!=NULL)
 {
 if(strcmp(word,n->word)<0)
 n=n->left;
 else if(strcmp(word,n->word)>0)
 n=n->right;
 else if(strcmp(word,n->word)==0)
 {
 flag=1;
 system("cls"); //to clear screen of all previous outputs
 cout<<"\n\nWord Found!";
 cout<<endl<<"Word: "<<n->word<<endl<<"Meaning: "<<n->meaning<<endl;
 cout<<"\nPress enter key to continue";
 getch(); //to ensure program waits for user to press a key before continuing to next step
 return;
 }
 }
 if(flag==0)
 {
 system("cls"); //to clear screen of all previous outputs
 cout<<"\nWord not found in dictionary \nPress enter key to return to main menu";
 getch(); //to ensure program waits for user to press a key before continuing to next step
 return;
 }
}
void inordertrav(bstnode t) //to perform in-order traversal of the tree with root t
{
 bstnode x=t;
 if(t!=NULL) 
 {
 inordertrav(t->left);
 cout<<"Word: "<<t->word<<endl<<"Meaning: "<<t->meaning<<endl<<endl;
 inordertrav(t->right);
 }
}
void display() //To display all words in the dictionary
{
 system("cls"); //to clear screen of all previous outputs
 int i;
 for(i=0; i<26; i++)
 {
 if(record[i]!=NULL)
 {
 cout<<"\nWords starting with "<<(char)(i+65)<<": \n";
 inordertrav(record[i]);
 }
 }
 cout<<"\nPress enter key to return to main menu";
 getch(); //to ensure program waits for user to press a key before continuing to next step
 return;
}
void reset() //to remove all user entered data and reset tree to universal dictionary only
{
 system("cls"); //to clear screen of all previous outputs
 cout<<"Reseting to universal file...\nPress enter key to return to main menu";
 getch(); //to ensure program waits for user to press a key before continuing to next step
 temp(); 
 return;
}
void export_dict() //to create a text file of the dictionary
{
 system("cls"); //to clear screen of all previous outputs
 char name[100];
 universal s;
 cout<<"Enter name for file to be exporting dictionary to(include desired extension): ";
 cin>>name;
 ofstream ofs(name, ios::out);
 ifstream ifs("universal.dat", ios::in | ios::binary);
 while(!ifs.eof())
 {
 ifs.read((char*)&s,sizeof(s));
 ofs<<s.word<<": "<<s.meaning<<endl;
 }
 ofs.close();
 ifs.close();
}
void subsearch(bstnode t, char sub[100])
{
 int l,i;
 char temp[100];
 l=strlen(sub);
 while(t!=NULL)
 {
 for(i=0;i<l;i++)
 { 
 temp[i]=t->word[i];
 }
 temp[i]='\0';
 if(strcmp(temp,sub)==0)
 {
 cout<<"Word: "<<t->word<<endl<<"Meaning: "<<t->meaning<<endl<<endl;
 subsearch(t->left,sub);
 subsearch(t->right,sub);
 t=NULL;
 }
 else if(strcmp(sub,temp)>0)
 {
 t=t->right;
 }
 else
 {
 t=t->left;
 }
 }
}
void search_sub() //
{
 system("cls"); //to clear screen of all previous outputs
 char ch,sub[100];
 int hf;
 cout<<"Enter substring to search for: ";
 cin>>sub;
 ch=sub[0];
 hf=int(ch)-97; 
 bstnode t;
 t=record[hf];
 subsearch(t,sub);
 getch(); //to ensure program waits for user to press a key before continuing to next step
}
int main_menu() //to give the user his options for running the program
{
 system("cls"); //to clear screen of all previous outputs
 int choice;
 cout<<"\n\tMain Menu\n\n";
 cout<<"\n01. Add words to dictionary";
 cout<<"\n02. Add user";
 cout<<"\n03. Delete user";
 cout<<"\n04. Reset dictionary";
 cout<<"\n05. Search for meaning in dictionary";
 cout<<"\n06. Export dictionary to text file";
 cout<<"\n07. Show dictionary";
 cout<<"\n08. Search for words with a substring";
 cout<<"\n0. Exit";
 cout<<"\n\nEnter your choice: ";
 cin>>choice;
 return choice;
}
int main() //to run and guide the program
{
 int choice;
 temp(); //to build the trees which shall store all the words from the users
 do 
 {
 choice=main_menu();
 switch(choice)
 {
 case 1: //Add new words to dictionary
 add_words_menu();
 break;
 case 2: //To create new user
 create_user();
 break;
 case 3: //To delete user data from file
 delete_user();
 break;
 case 4: //To reset the dictionary to the universal state
 reset();
 break;
 case 5: //To search the dictionary for a word and it's meaning
 search_word();
 break;
 case 6: //To export the dictionary to a text file so that user can access it without program
 export_dict();
 break;
 case 7: //To display all the words in the dictionary
 display();
 break;
 case 8: //To search for all words starting with given substring
 search_sub();
 break;
 case 0: //To exit from program
 choice=exit_menu();/*choice value is modified to 1 if user wants to return to 
 program so that the do-while condition is satisfied.*/
 break;
 default:
 {
 system("cls"); //to clear screen of all previous outputs
 cout<<"Wrong input!\nTry Again...";
 getch(); //to ensure program waits for user to press a key before continuing to next step
 }
 }
 }while(choice!=0);
 return 0;
} 

