import java.util.*;

import javax.naming.StringRefAddr;

import java.io.*;
public class temp {

	public static void main(String[] args) {
		
		int count=0;
		String name,temp;
		
		FileOutputStream fout;
		FileInputStream fin;
		File file;
		Scanner scn = new Scanner(System.in);
		String data;		
		
		
		try{
			
			int ch;
			System.out.println("1. append /n 2. delete");
			ch = scn.nextInt();
			fin = new FileInputStream("D://1/version.txt");
			count = fin.read()-48;
			name = "D://1/temp_version "+count+".txt";
			temp = "D://1/temp_version "+ (count-1) +".txt";
			file = new File(name);
		if(file.createNewFile()){
			System.out.println("created");
		}
		else{
			System.out.println("no");
		}
		
		switch(ch){
		case 0:
			fout = new FileOutputStream(file);
			data = scn.next();
			fout.write(data.getBytes());
			break;
		case 1 :
				fin = new FileInputStream(temp);
				fout = new FileOutputStream(file);
				int i ;
				do{
					i = fin.read();
					if(i!=-1){
						fout.write(i);
					}
				}while(i!=-1);
				//fout.write(" ");
				data = scn.next();
				fout.write(data.getBytes());
				break;
		case 2 :
			String comm =scn.next();
			char j = comm.charAt(3); 
			System.out.println(j);
			String name_temp = "D://1/temp_version "+ j +".txt";
			fin = 
			
			
			new FileInputStream(name_temp);
			int k = 0;
			String out="\n";
			do{
				k = fin.read();
				if(k!=-1){
					out = out + k;
				}
			}while(k!=-1);
		
			System.out.println(out);
			break;
		}
		
		
		count=count + 48;
		count++;
			fout = new FileOutputStream("D://1/version.txt");
			fout.write(count);
		}catch(IOException e){
			e.printStackTrace();
		}
	}
	}
