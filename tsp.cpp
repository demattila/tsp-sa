#include <iostream>                                  
#include <vector>
#include <sstream>
#include <string>
#include <cctype>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

typedef vector <int> ivec;                          
typedef vector <string> svec;
typedef vector <double> dvec;


int stringToInt(string numStr);                      
double distPoints(int x1, int y1, int x2, int y2);
double total_Distance(vector <double> distances);

int main()
{
	string filename = "coord.txt";                              
	ifstream infile;                        
	do
	{
		infile.open(filename.c_str(), ifstream::in);
		if (!infile.is_open())                        
		{
			cout << "Could not open " << filename << endl;  
			cout << "Enter filename" << endl;             
			getline(cin, filename);
		}
		else
			break;                                 
	} while (true);
	dvec xCoords;
	dvec yCoords;
	int coord_counter = 1;
	while (infile.good())                           
	{
		int coord;
		string numStr;                               
		infile >> numStr;                                              
		coord = stringToInt(numStr);          
		if (coord_counter % 2 != 0)            
		{
			xCoords.push_back(coord);        // x coordinate
		}
		else if (coord_counter % 2 == 0)         
		{
			yCoords.push_back(coord);       //y coordinate
		}
		coord_counter++;                   
	}
	if (xCoords.size() > yCoords.size())    //if uneven number of points
	{
		cout << "Error with the coordinates" << endl;  
		return 0;
	}
	cout << "Initial Path:" << endl;                     
	for (int index = 0; index < xCoords.size(); index++)  
	{
		cout << xCoords[index] << "," << yCoords[index] << endl;
	}
	if (xCoords.size() == 1)                         
	{
		cout << "There is only one point." << endl;
		return 0;
	}
	else if (xCoords.size() <= 3 && xCoords.size() > 1)
	{
		cout << "Not enough points" << endl;
		return 0;
	}

	dvec all_Initial_Distances; 
	for (int x = 0; x < xCoords.size(); x++)
	{
		if (x == xCoords.size() - 1)  
		{
			int exeption = distPoints(xCoords[x], yCoords[x], xCoords[0], yCoords[0]);  //must come back to first point
			all_Initial_Distances.push_back(exeption);   
			break;
		}
		int initial_Distance = distPoints(xCoords[x], yCoords[x], xCoords[x + 1], yCoords[x + 1]);  //otherwise go from point to next
		all_Initial_Distances.push_back(initial_Distance);
	}

	double initial_Cost = total_Distance(all_Initial_Distances);

	double good_Cost;
	good_Cost = initial_Cost;
	double goal_Cost = good_Cost * 0.4;    //reduce intial route by 60%
	double  current_Cost = initial_Cost + 1;                  
	dvec all_Current_Paths;
	int maxProbability = 31128;                         
	srand(time(NULL));
	while (current_Cost > goal_Cost || maxProbability <= 10) //low probability
	{
		int change = rand() % xCoords.size();        
		int change2 = rand() % xCoords.size();
		int b = xCoords[change];        
		xCoords[change] = xCoords[change2];
		xCoords[change2] = b;
		int a = yCoords[change];        
		yCoords[change] = yCoords[change2];
		yCoords[change2] = a;


		for (int q = 0; q < xCoords.size(); q++)
		{
			if (q == xCoords.size() - 1)  
			{
				int final_step = distPoints(xCoords[q], yCoords[q], xCoords[0], yCoords[0]);  
				all_Current_Paths.push_back(final_step); 
				break;
			}
			int current_Path = distPoints(xCoords[q], yCoords[q], xCoords[q + 1], yCoords[q + 1]);
			all_Current_Paths.push_back(current_Path);
		}

		current_Cost = total_Distance(all_Current_Paths);

		if (current_Cost < good_Cost)                        //if new route is better assigns it as default
		{
			good_Cost = current_Cost;                        
		}

		else if (current_Cost >= good_Cost)
		{
			double percentage_Worse = good_Cost / current_Cost; 
			double specificProbability = maxProbability * percentage_Worse; 
			int cap = 32767;                                  
			int decider = rand() % cap + 1;                     
			if (decider <= specificProbability)
			{
				good_Cost = current_Cost;
			}
			if (decider > specificProbability)
			{
				int b = xCoords[change2];        
				xCoords[change2] = xCoords[change];
				xCoords[change] = b;
				int a = yCoords[change2];       
				yCoords[change2] = yCoords[change];
				yCoords[change] = a;
			}
		}

		all_Current_Paths.clear();

		maxProbability -= 25;                            
		if (maxProbability <= 10)                        //probability is low
		{
			break;
		}
	}
	cout << "The new order:" << endl;          
	for (int c = 0; c < xCoords.size(); c++) 
	{
		cout << xCoords[c] << "," << yCoords[c] << endl; 
	}

	cout << "Original distance: " << initial_Cost << endl; 
	cout << "New total distance: " << good_Cost << endl;
	float improvement = 100 - ((good_Cost / initial_Cost) * 100);
	cout << "Improvement: " << improvement << "%" << endl;
	system("pause");
	return 0;
}

double distPoints(int x1, int y1, int x2, int y2)
{
	double distance = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
	return distance;                                   
}

double total_Distance(vector <double> distances)
{
	double total = 0;                                      
	for (int i = 0; i < distances.size(); i++)           
	{
		total += distances[i];                         
	}
	return total;                                      
}
int stringToInt(string numStr)
{
	int num = 0;
	bool isSuccessful = true;
	int len = numStr.length();
	for (int i = 0; i < len; i++)
	{
		char ch = numStr[i];
		int digit;
		if (isdigit(ch))
		{
			stringstream ss;
			ss << ch;
			ss >> digit;
			num *= 10;
			num += digit;
		}
	}
	if (numStr[0] == '-')
	{
		num = num * -1;
	}
	if (isSuccessful)
	{
		return num;
	}
	return 0;
}



