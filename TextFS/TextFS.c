/*
 Textfs - A text based file system
Mr. Ramesh, programmer by profession is very obsessed with his room partner Mr. Suresh.  Suresh leaves no opportunity to break into and check out important and private files of Ramesh. One fine day Ramesh decides to implement his own file system Textfs that could store important files without revealing much information about them. Textfs will be a text based file system meaning all the data regarding contents of file system(meta-data) and the actual data of files will go in a single text file. The only principle that this file system is based on is - simplicity.
As the only factor under consideration is simplicity and not efficiency he decides to write a user mode command line application. This application has following major goals :
1.  Create a new file (create command)
2. Copy content of external files into these newly created files (copy command)
3.  Print contents of internal files (echo command)
4. Delete existing file (delete command)
5. List all files (ls command)
Again in order to keep things simple he agrees on some limitations:
1. Application will only create/copy/print *text* files
2. Application will only allow copying contents from external files(eg : /usr/readme.txt can be copied to readme.txt created by the application); it won't allow editing of contents.
3. Application will act as a command interpreter and accept only five commands create, copy, echo, ls  and delete with relevant parameters ; all text files directly go into Textfs without any folder structure.
4. Textfs will store all the contents inside a single file meaning the minimal super block, the minimal inode list and the storage block will all be placed inside dynamically increasing text file.
Textfs will allow Ramesh to hide all private text files unless and until Suresh gets hold of the application.
Please help Ramesh in designing and implementing this file system.
You are free to standard super block - inode list - data block design or any other design as you wish. You are free to choose any method for storing data blocks only constraint being simplicity.
(NOTE : Standard super block and inode list will have hundreds of fields but we are only concerned about a few here. It is advisable not to look at these structures and design your own from scratch; marks will be allotted to design as a whole and not individual efficient algorithms. Also we are nowhere concerned about security and this is in no way related to cryptography- the science of data scrambling. As you design this system you will automatically realize how it hides the text files beneath Textfs) .
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
	FILE *textfs;
	char ch, copyCh;
	char tempData[200], tempCount[5];
	char filename[20]; 
	int charCount;
	int fileFound = 0; 		//flag...
	
	FILE *replicaTextFS;	//for delete and copy operation...
	FILE *copySrc;			//for src file, from where to copy...
	char copyPath[50];		//path of src file...
	int choice, copyCount;				//in copy, either merge or replace data...
	
	switch(argc)
	{
		case 2: 
				//ls operation...
				
				if(!strcmp(argv[1], "ls")){
					
					textfs = fopen("textFS", "r");
					
					while(1){
						
						fscanf(textfs, "%s", tempData);
						
						printf("%s\n", tempData);
						
						fscanf(textfs, "%s", tempCount);
						charCount = atoi(tempCount);
						
						charCount++;
						fseek(textfs, charCount, SEEK_CUR);	
					
						//printf("charcount : %d\n", charCount);
					
						ch = getc(textfs);	//avoid infinite loop, to get the EOF character.
						if(ch == EOF) break;
						
					}
						
					fclose(textfs);	
					
				}else{
					printf("One arg. Usage ->\n");
					printf("to list files : ls\n\n");
				}
				
				break;
		case 3:
				//create, echo, delete operations...
				
				strcpy(filename, argv[2]);
								
				//create...
				if( !strcmp(argv[1],"create") ){
						
						printf("Enter the data (press 'tab then Enter' to stop)--->\n");
						scanf("%[^\t]s", tempData);
						charCount = strlen(tempData);
						
						textfs = fopen("textFS", "a");
						
						fprintf(textfs, "%s %d %s ", filename, charCount, tempData);
						
						fclose(textfs);
						
				}	//echo...
				else if( !strcmp(argv[1],"echo") ){
					
					textfs = fopen("textFS", "r");
					fileFound = 0;
					
					while(1){
						
						fscanf(textfs, "%s", tempData);
						
						if( !strcmp(filename, tempData) ){
							fileFound = 1;
							break;
						}else{
							ch = getc(textfs);
							if(ch == EOF)
								break;
						}
					}
					
					if(fileFound){
						fscanf(textfs, "%s", tempCount);
						charCount = atoi(tempCount);
						charCount++;	//to add last char
						printf("the file Data->\n");
					
						while( charCount-- ){
							ch = getc(textfs);
							printf("%c", ch);
						}
						printf("\n");
					}else
						printf("Requested file not present!\n");
						
					fclose(textfs);		
				
				}	//delete...
				
				else if( !strcmp(argv[1], "delete") ){
				
					textfs = fopen("textFS", "r");
					replicaTextFS = fopen("replica", "w");
					
					while(1){
						
						fscanf(textfs, "%s", tempData);
						fscanf(textfs, "%s", tempCount);
						charCount = atoi(tempCount);
							
						//file to be deleted is found...
						if( !strcmp(tempData, filename) ){
							
							charCount += 1;		//to consider space after count value...
							fseek(textfs, charCount, SEEK_CUR);
							
							/*
							ch = ' ';
							putc(ch, replicaTextFS);	//adjustment for correctness... ;p
							*/
							while( (ch = getc(textfs)) != EOF){
								putc(ch, replicaTextFS);
							}
							break;	//EOF reached, break from while(1)...
												
						}	//file to be deleted is not this file...
						else{
								fprintf(replicaTextFS, "%s ", tempData);
								fprintf(replicaTextFS, "%s", tempCount);
								
								charCount++;	//to consider last char space also...
								while(charCount--){
									
									ch = getc(textfs);
									putc(ch, replicaTextFS);	
								
								}
								if(ch == EOF)
									break;
							}
						}
						
					fclose(textfs);
					fclose(replicaTextFS);
										
					
					//remove old textFS file and use newly created file...
					remove(filename);
					rename("replica", "textFS");
					
				
				}	//invalid input...
				
				else{
					printf("Usage -> \n");
					printf("To create : create <filename>\n");
					printf("To print : echo <filename>\n");
					printf("To delete : delete <filename>\n\n");
				}
				
				break;
		case 4: 
				//copy operation...
				if( !strcmp(argv[1], "cp") )
				{
					
					strcpy(copyPath, argv[2]);
					strcpy(filename, argv[3]);
					
					fileFound = 0;
					
					textfs = fopen("textFS", "r");
					copySrc = fopen(copyPath, "r");
					replicaTextFS = fopen("replica", "w");
					
					copyCount = 0; 
					while ( (ch = getc(copySrc)) != EOF)
						copyCount++;
					
					copyCount--;		//to exclude EOF character...
					
					rewind(copySrc);		//bring pointer to start of file
					
					
					
					while(1){
						
						fscanf(textfs, "%s", tempData);
						
						//file in which copy, is found...
						if( !strcmp(tempData, filename)){
						
								printf("\nMerge{1} or Replace{2} ? : ");
								scanf("%d", &choice);
								//printf("choice : %d", choice);
								
								switch(choice){
									case 1:
											//merge data...
											
											fprintf(replicaTextFS, "%s ", tempData);
											
											fscanf(textfs, "%s", tempCount);
											//fprintf(replicaTextFS, "%s", tempCount);
											charCount = atoi(tempCount);
											fprintf(replicaTextFS, "%d", charCount+charCount);	//updated block size
											
											charCount++;
											while( charCount--){
												ch = getc(textfs);
												putc(ch,replicaTextFS);
											}
											
											putc(' ', replicaTextFS);
											
											while( (ch=getc(copySrc)) != EOF){
												//printf("hora!");
												putc(ch, replicaTextFS);
											}
											
											putc(' ', replicaTextFS);
											
											//to avoid extra '\n'
											ch = getc(textfs);
											ch = getc(textfs);
											//--------------------
											while( (ch=getc(textfs)) != EOF){
												putc(ch, replicaTextFS);
											}
											
											fileFound = 1;
											break;
									
									case 2: //replace data...
											
											fprintf(replicaTextFS, "%s ", tempData);
											
											fscanf(textfs, "%s", tempCount);
											//fprintf(replicaTextFS, "%s", tempCount);
											charCount = atoi(tempCount);
											fprintf(replicaTextFS, "%d", copyCount);
											
											
											/*charCount++;
											while( charCount--){
												ch = getc(textfs);
												putc(ch,replicaTextFS);
											}*/
											
											putc(' ', replicaTextFS);
											
											while( (ch=getc(copySrc)) != EOF){
												putc(ch, replicaTextFS);
											}
											
											putc(' ', replicaTextFS);
											
											charCount++;
											fseek(textfs, charCount, SEEK_CUR);
											
											//to avoid extra '\n'
											ch = getc(textfs);
											ch = getc(textfs);
											//---------------
											while( (ch=getc(textfs)) != EOF){
												putc(ch, replicaTextFS);
											}
											
											fileFound = 1;
											break;
								}
							if(fileFound) break;	//infinite loop break;	
						}	//file for copy - not matched...
						else{
							
							fprintf(replicaTextFS, "%s ", tempData);
							fscanf(textfs, "%s", tempCount);
							fprintf(replicaTextFS, "%s", tempCount);
							charCount = atoi(tempCount);
							
							charCount++;
							while( charCount--){
								ch = getc(textfs);
								putc(ch,replicaTextFS);
							}
							
							if(ch == EOF) break;		//infinite loop break;
						}
						
					}
					
					fclose(textfs);
					fclose(copySrc);
					fclose(replicaTextFS);
					
					remove(filename);
					rename("replica", "textFS");
					
				
				}
				else{
				
					printf("Usage -> \n");
					printf("To copy : cp <src_filename> <des_filename>\n\n");
				}
				break;
	}
	
	return 0;
	
}
