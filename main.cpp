#include<iostream>
#include<math.h>
#include "md5.h"
#include <sstream>
#include "pastry.h" 

using namespace std;
int dataInserted = 0;
vector<string> dataToBeStored;
bool makeReport = false; //make it true to evaluate the network, otherwise false will run the network acc to user inputs


void option_insert(Pastry* p){
	int choice = 0;
	cout<<"Choice to insert node: "<<endl;
	//cout<<"1. Randomly Chosen."<<endl;
	cout<<"1. Given an IPAddress an location:"<<endl;
	cout<<"2. From one of prev deleted nodes: "<<endl;
	
	cin>> choice;
	pastryNode* newNode = NULL;
	
	if(choice == 3){
		newNode = p -> generateNode();
		if(!newNode){
		 	cout<<"Cannot create"<<endl;
		 	return;
		}
		p -> insertNodeInPastryNetwork(newNode, true);
		p -> numOfNodes++;
		cout<<"Node Inserted"<<endl;	
	}else if(choice == 1){
		newNode = p -> initializeAGivenNode();
		if(newNode == NULL){
			cout<<"Node with IP address or location already present"<<endl;
			return;
		}
		p -> insertNodeInPastryNetwork(newNode, true);
		cout<<"Node Inserted"<<endl;
	}else if(choice == 2){
		bool t = p -> displayAllDeletedNodes();
		if(!t) return;
		
		int nodeNum;
		cout<<"Enter the node number you want to insert: "<<endl;
		cin >> nodeNum;
		
		newNode = p -> getDeletedNode(nodeNum);
		
		p-> insertNodeInPastryNetwork(newNode, true);
		cout<<"Node Inserted"<<endl;
	}
	
	p -> numOfAddQueries++;
	
}

void option_delete(Pastry* p){
	
	string nodeID;
	int num;
	cout<<"Enter a number of node you want to remove: "<<endl;
	cin >> num;
	int i = 0;
	for(auto it = Pastry::mapIdNode.begin(); it != Pastry::mapIdNode.end(); it++){
		if(i == num) {
			nodeID = it -> first;
			break;
		}	
		i++;
	}
	cout<<nodeID;
	//bool s = 
	p -> deleteNodeInPastry(nodeID);
	
	//if(s) cout<<nodeID<<" was deleted from network";
	//else cout<<"hdjk";
	p -> numOfDeleteQueries++;
}

void option_lookup(Pastry* p){
	int choice;
	cout<<"1. Look up Node"<<endl;
	cout<<"2. Look up data"<<endl;
	string keyID;
	cin>>choice;
	if(choice == 1){
		cout<<"Enter the Node ID you want to look up:"<<endl;
		cin>>keyID;
		bool exists = p -> lookUpInPastryNetwork(keyID, true, false, 1);
		if(!exists) cout<<"The NodeID you entered doesn't exists";
	}else if(choice == 2){
		string data = "";
		cout<<"Enter the data you want to look up:"<<endl;
		cin>>data;
		keyID = md5(data);
		bool exists = p -> lookUpInPastryNetwork(keyID, true, true, 1);
		if(!exists) cout<<"The key you entered doesn't exists";
		p -> numOfSearchQueries++;
	}
	
}				

void option_keyvalue(Pastry* p){
	string data;
	cout<<"Enter the data you want to insert: ";
	cin >> data;
	p -> setDataOnAppNode(data, false, "");
	p -> numOfDataAddQueries++;
}


void option_prints(Pastry* p){
	cout<<"Total number of nodes: "<<p -> mapIdNode.size()<<endl;
	cout<<"Total number of data elements: "<< p -> numOfDataAddQueries<<endl;
	cout<<"Total search queries: "<< p -> numOfSearchQueries<<endl;
	cout<<"Total node add queries: "<< p -> numOfDataAddQueries<<endl;
	cout<<"Total node delete queries: "<< p -> numOfDeleteQueries<<endl;
	cout<<"Total data add queries: "<< p -> numOfDataAddQueries<<endl;
	cout<<endl;
	
}

void option_printRT(Pastry* p){
	string nodeID;
	
	cout<<"Enter The Node ID of which you want to print Routing Table: "<<endl;
	cin>>nodeID;
	
	option_prints(p);
	cout<<"Routing Table. ID: "<<nodeID;
	
	pastryNode* pNode = Pastry::getActiveNodeWithID(nodeID);
	
	pNode -> printRoutingTable();

}



void generateAllStrings(vector<char>& a, int size, string prefix, int k, int total){
	if(dataToBeStored.size() == total) return;
	if( k == 0 ){
		dataToBeStored.push_back(prefix);
		
		return;
	}
	
	for(int i = 0; i < size; i++){
		string newPrefix;
		newPrefix = prefix + a[i];
		
		generateAllStrings(a, size, newPrefix, k - 1, total);
	}
}

void populateNetworkWithData(Pastry* p){
	int prevRandom = -1;
	srand((unsigned) time(0));
	for(int i = 0; i < dataToBeStored.size(); i++){
		string data = dataToBeStored[i];
		//we choose a random node from a network using which we
		// find appropriateplace to store data
		
		int randNodePos = (rand() % Pastry::mapIdNode.size());
		while(randNodePos == prevRandom){
			randNodePos = (rand() % Pastry::mapIdNode.size());
		}
		prevRandom = randNodePos;
		
		p -> setDataOnAppNode(data, false, to_string(randNodePos));
	}
}


void generateData(int numOfDataPoints){
	vector<char> a;
	
	for(int i = 65; i < 91; i++){
		char tmp = i;
		a.push_back(tmp);
	}
	generateAllStrings(a, 91-65, "", 3, numOfDataPoints);
}


void queryTheDataPresent(Pastry* p, int num){
	int prevRandom = -1;
	int prevRandomNode = -1;
	srand((unsigned) time(0));
	for(long int i = 0; i < num; i++){
		//we select a random data to query
		int randData = (rand() % dataToBeStored.size());
		while(randData == prevRandom){
			randData = (rand() % dataToBeStored.size());
		}
		prevRandom = randData;
		
		int randNodePos = (rand() % Pastry::mapIdNode.size());
		while(randNodePos == prevRandomNode){
			randNodePos = (rand() % Pastry::mapIdNode.size());
		}
		prevRandomNode = randNodePos;
		cout<<randData<<" "<<randNodePos;
		string key = dataToBeStored[randData];
		string keyID = md5(key);
		bool exists = p -> lookUpInPastryNetwork(keyID, true, true, randNodePos);
		if(!exists) cout<<"entry: "<<keyID<<" was not found"<<endl;
		
	}

}

void deleteNodesRandomly(Pastry* p, int num){
	int prevRandomNode = -1;
	srand((unsigned) time(0));
	for(int i = 0; i < num ; i++){
		int randNodePos = (rand() % Pastry::mapIdNode.size());
		while(randNodePos == prevRandomNode){
			randNodePos = (rand() % Pastry::mapIdNode.size());
		}
		prevRandomNode = randNodePos;
		cout<<"Node Deleted"<<endl;
		string nodeID = "";
		int j = 0;
		for(auto it = Pastry::mapIdNode.begin(); it != Pastry::mapIdNode.end(); it++){
			if(j == randNodePos){
				nodeID = it -> first;
				break;
			} 
		}
		p -> deleteNodeInPastry(nodeID);
		
	}

}


int main(int argc, char* argv[]){
	
	if(!makeReport){
		Pastry* p = new Pastry(100, 4, 10, 10);
		p -> printAllCurrentNodes();
		p -> initializePastry();
	
		map<string, pastryNode*> ::iterator it = Pastry::mapIdNode.begin();
		/* To print the tables of initialized nodes
		*/for(; it!= Pastry::mapIdNode.end(); it++){
			pastryNode* tmp = it -> second;
			
			tmp -> printLeafSet();
			tmp -> printNeighbourhoodSet();
			tmp -> printRoutingTable();
		}
	
		int option;
		cout<<"Opearations For Pastry DHT:"<<endl;
		cout<<"1. Insert a node in network"<<endl;
		cout<<"2. Delete a node in network"<<endl;
		cout<<"3. Look up data with key"<<endl;
		cout<<"4. Add a key-value pair"<<endl;
		cout<<"5. Print Routing table of a Node"<<endl;
		cout<<"6. Print Network summary"<<endl;
		cout<<"7. Exit"<<endl;
		
	while(true){
			
		cout<<"Enter an option:"<<endl;
		cin >> option;
	
		switch(option){
			case 1:
				cout<<"Node Insertion Selected"<<endl;
				option_insert(p);
				break;
			case 2:
				cout<<"Node deletion selected"<<endl;
				option_delete(p);
				break;
			case 3:
				//cout<<"Look up data Selected"<<endl;
				option_lookup(p);
				break;
			case 4:
				cout<<"Add a key-value pair selected"<<endl;
				option_keyvalue(p);
				break;
			case 5:
				cout<<"Print a routing table of a node selected"<<endl;
				option_printRT(p);
				break;
			case 6:
				cout<<"Print summary of network selected"<<endl;
				option_prints(p);
				break;
			case 7:
				cout<<"Exiting.."<<endl;
				exit(0);
				break;
			default:
				cout<<"Non-identifiable Input";
				break;
			}	
		}
	}else{
	
		Pastry* p = new Pastry(1000, 4, 10, 10);
		//p -> printAllCurrentNodes();
		p -> initializePastry();
	
		map<string, pastryNode*> ::iterator it = Pastry::mapIdNode.begin();
		
		
		for(; it!= Pastry::mapIdNode.end(); it++){
			pastryNode* tmp = it -> second;
			tmp -> printLeafSet();
			//tmp -> printNeighbourhoodSet();
			tmp -> printRoutingTable();
		}
		
		generateData(10000);
		populateNetworkWithData(p);
		cout<<"TOTAL DATA ITEMS IN WHOLE NETWORK: "<<dataToBeStored.size()<<endl;
		
		deleteNodesRandomly(p, 500);
		
		queryTheDataPresent(p, 1000000);
		float sum = 0;
		int num0 = 0, num1= 0, num2 = 0, num3 = 0, num4 = 0, num5 = 0, num6 = 0, other = 0;
		
		for(int i = 0; i < p -> hopsSetData.size(); i++){
			//cout<<p -> hopsGetData[i];
			sum+=p -> hopsGetData[i];
			if(p ->hopsSetData[i] == 0) num0++;
			else if(p ->hopsSetData[i] == 1) num1++;
			else if(p ->hopsSetData[i] == 2) num2++;
			else if(p ->hopsSetData[i] == 3) num3++;
			else if(p ->hopsSetData[i] == 4) num4++;
			else if(p ->hopsSetData[i] == 5) num5++;
			else if(p ->hopsSetData[i] == 6) num6++;
			else other++;
		}
		
		
		cout<<"Number of 0 hops: "<<num0<<endl;
		cout<<"Number of 1 hops: "<<num1<<endl;
		cout<<"Number of 2 hops: "<<num2<<endl;
		cout<<"Number of 3 hops: "<<num3<<endl;
		cout<<"Number of 4 hops: "<<num4<<endl;
		cout<<"Number of 5 hops: "<<num5<<endl;
		cout<<"Number of 6 hops: "<<num6<<endl;
		cout<<"Number of other hops: "<<other<<endl; 
		cout<<"Number of found: "<<p ->hopsSetData.size();
		cout<<"average: "<<sum/p -> hopsSetData.size();
		
		cout<<endl;
		cout<<"For get Data"<<endl;
		
		for(int i = 0; i < p -> hopsGetData.size(); i++){
			//cout<<p -> hopsGetData[i];
			sum+=p -> hopsGetData[i];
			if(p ->hopsGetData[i] == 0) num0++;
			else if(p ->hopsGetData[i] == 1) num1++;
			else if(p ->hopsGetData[i] == 2) num2++;
			else if(p ->hopsGetData[i] == 3) num3++;
			else if(p ->hopsGetData[i] == 4) num4++;
			else if(p ->hopsGetData[i] == 5) num5++;
			else if(p ->hopsGetData[i] == 6) num6++;
			else other++;
		}
		
		
		
		cout<<"Number of 0 hops: "<<num0<<endl;
		cout<<"Number of 1 hops: "<<num1<<endl;
		cout<<"Number of 2 hops: "<<num2<<endl;
		cout<<"Number of 3 hops: "<<num3<<endl;
		cout<<"Number of 4 hops: "<<num4<<endl;
		cout<<"Number of 5 hops: "<<num5<<endl;
		cout<<"Number of 6 hops: "<<num6<<endl;
		cout<<"Number of other hops: "<<other<<endl; 
		cout<<"Number of found: "<<p ->hopsGetData.size();
		cout<<"average: "<<sum/p -> hopsGetData.size();
	}
}
