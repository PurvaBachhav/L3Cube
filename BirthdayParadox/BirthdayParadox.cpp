/*
 Is Birthday paradox really valid? 
----------------------------------------- 
 
Write a code that verifies - birthday paradox is indeed correct. 
 
(Note : Think of ways you could run 'random experiments'. We agree this problem is hard.)  
 
*/
#include <iostream>
#include <cstdlib>

using namespace std;

int main()
{
	int tc;
	
	/*cout<<"Enter how many time you want to verfiy : ";
	cin>>tc;
	
	while(tc--)
	{
	*/
		//theoretical probability calculation...
		int persons;
	
		cout<<"Enter the number of persons : ";
		cin>>persons;
	
		int total_pairs;
		total_pairs = persons * (persons-1) / 2;
		cout<<total_pairs; 
		double prob_one_not_matching = (double)364/365;
		double prob_all_not_matching = 1;
	
		for(int i=0; i<total_pairs; i++)
			prob_all_not_matching *= prob_one_not_matching;
		
		double prob_all_matching = 1 - prob_all_not_matching;
		cout<<"Theoretical probability calculation method:\n";
		cout<<"Probability of "<<persons<<" persons having same birthdays is : "<<prob_all_matching<<"\n";
		
		//practical example to verify...
		
		int trials;
		
		cout<<"number of trials : ";
		cin>>trials;
		int match=0;
		int n=1;
		while(trials--)
		{
			int persons_birthday[persons];
			
			for(int i=0; i<persons; i++)
			{
				persons_birthday[i] = (rand()%365)+1;	//random number between 1-365 representing the birthday...			
			}
			int count_array[365] = {0};
			
			for(int i=0; i<persons; i++){
				count_array[persons_birthday[i]]++;
			}
			
			for(int i=0; i<365; i++)
				if(count_array[i] > 1)
				{
					match++;
					break;
			 	}	
			cout<<"Test "<<n<<":";	
			cout<<"For "<<persons<<" persons -> "<<match<<" pairs have same birthday!\n";
		
			n++;
		}
		cout<<"For "<<persons<<" persons -> "<<match<<" pairs have same birthday!\n";
		
	
	//}
	return 0;
}
