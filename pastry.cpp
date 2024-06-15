
#include "pastry.h"

using namespace std;



map<string, pastryNode*> Pastry::mapIdNode;

	
Pastry::Pastry(int n, int b, int L, int M){
		numOfNodes = n;
		bval = b; 
		//this -> row = ceil(log(numOfNodes)/log(pow(2, bval)));
		this -> row = 32;
		this -> column = pow(2, bval);
		this -> L = L;
		this -> M = M;
		this -> nodeIDLength = row;
		
		this -> numOfSearchQueries = 0;
		this -> numOfAddQueries = 0;
		this -> numOfDeleteQueries = 0;
		this -> numOfDataAddQueries = 0;
		
		//Pastry is initialized so we create empty nodes the first node
		srand((unsigned) time(0));
		int ipAddress[4];
		int gpsLocation[2];
		
		//initialize all pastry nodes with random ip address and locations
		for(int i = 0; i < numOfNodes; i++){
			int ipOctet1 = (rand() % 256);
			int ipOctet2 = (rand() % 256);
			int ipOctet3 = (rand() % 256);
			int ipOctet4 = (rand() % 256);
			
			//cout<<ipOctet1<<" ";
			while(isIPpresent(ipOctet1, ipOctet2, ipOctet3, ipOctet4, i)){
				ipOctet1 = (rand() % 256);
				ipOctet2 = (rand() % 256);
				ipOctet3 = (rand() % 256);
				ipOctet4 = (rand() % 256);
				
			}
			
			ipAddress[0] = ipOctet4;
			ipAddress[1] = ipOctet3;
			ipAddress[2] = ipOctet2;
			ipAddress[3] = ipOctet1;
			
			
			int latitude = ((rand() % 180) + 1 ) - 90;
			int longitude = ((rand() % 360) + 1 ) - 180;
			
			while(islocationPresent(latitude, longitude, i)){
				latitude = ((rand() % 180) + 1 ) - 90;
				longitude = ((rand() % 360) + 1 ) - 180;
			
			}
			
			gpsLocation[0] = latitude;
			gpsLocation[1] = longitude;
			
			pastryNode* newNode = new pastryNode(row, column,ipAddress, gpsLocation, L, M);
			nodeList.push_back(newNode);
			//cout<<"size: "<<nodeList.size()<<":size ";
		}
		
		//cout<<"here:"<<row << " "<<column <<" "<<L << M;
		
	}
	
	
bool Pastry::isIPpresent(int ipOctet1, int ipOctet2, int ipOctet3, int ipOctet4, int n){
		
		for(int i = 0; i < n; i++){
			if(nodeList[i] -> getIpAddress(3) == ipOctet1){
				if(nodeList[i] -> getIpAddress(2) == ipOctet2){
					if(nodeList[i] -> getIpAddress(1) == ipOctet3){
						if(nodeList[i] -> getIpAddress(0) == ipOctet4) return true;
					}
				}
			}
		}
		return false;	
	}
	
bool Pastry::islocationPresent(int lat, int lon, int n){
		for(int i = 0; i < n; i++){
			if(nodeList[i] -> getGpsLocation(0) == lat){
				if(nodeList[i] -> getGpsLocation(1) == lon) return true;
			}
		}
	 	return false;
}
	
	
bool Pastry::initializePastry(){
		
		//first find node id from ipadress
		for(int i = 0; i < numOfNodes; i++){
			string ipAddress = to_string(nodeList[i] ->getIpAddress(0));
			ipAddress += to_string(nodeList[i] -> getIpAddress(1));
			ipAddress += to_string(nodeList[i] ->getIpAddress(2));
			ipAddress += to_string(nodeList[i] ->getIpAddress(3));
			
			string nodeID = md5(ipAddress);
			//string nodeID = "ff89a8701557a0ca2a26b5dd8fa760e";
			//nodeID += to_string(i);
			//cout << "node id generated"<<i<<": "<<nodeID<<endl;
			
			nodeList[i] -> setNodeID(nodeID.substr(0,row));
			
		}
		//insert each node by calling inseert node seperately and not initilaizing all at once.
		
		//for all nodes initialize leaf set and neighborhood set
		for(int i = 0; i < numOfNodes; i++){
			bool isInserted = insertNodeInPastryNetwork(nodeList[i], false);
			cout<<nodeList[i] -> getNodeID()<<" is inserted"<<endl;
			
		}
		
		
		
		return true;
	
	}	
	
	
bool Pastry::findIfNodeExistsInNetwork(string node){
		
		map <string, pastryNode*>:: iterator it;
		if(Pastry::mapIdNode.find(node) == Pastry::mapIdNode.end()) return false;
		else return true;
}

pastryNode* Pastry::generateNode(){
	int ipAddress[4];
	int gpsLocation[2];
	
	int ipOctet1 = (rand() % 256);
	int ipOctet2 = (rand() % 256);
	int ipOctet3 = (rand() % 256);
	int ipOctet4 = (rand() % 256);
			
	//cout<<ipOctet1<<" ";
	while(isIPpresent(ipOctet1, ipOctet2, ipOctet3, ipOctet4, numOfNodes)){
		ipOctet1 = (rand() % 256);
		ipOctet2 = (rand() % 256);
		ipOctet3 = (rand() % 256);
		ipOctet4 = (rand() % 256);
				
	}
			
	ipAddress[0] = ipOctet4;
	ipAddress[1] = ipOctet3;
	ipAddress[2] = ipOctet2;
	ipAddress[3] = ipOctet1;
			
			
	int latitude = ((rand() % 180) + 1 ) - 90;
	int longitude = ((rand() % 360) + 1 ) - 180;
			
	while(islocationPresent(latitude, longitude, numOfNodes)){
		latitude = ((rand() % 180) + 1 ) - 90;
		longitude = ((rand() % 360) + 1 ) - 180;
			
	}
			
	gpsLocation[0] = latitude;
	gpsLocation[1] = longitude;
	/*		
	pastryNode* newNode = new pastryNode(row, column,ipAddress, gpsLocation, L, M);
	nodeList.push_back(newNode);
	
	return newNode;*/
	return NULL;
}

bool pastryNode::notInPath(string nodeID, vector<pastryNode*>& route){
	for(int i = 0; i < route.size(); i++){
		if(route[i]-> getNodeID() == nodeID) return false;
	}
	return true;
}

bool Pastry::lookUpInPastryNetwork(string keyID, bool printRoute, bool data, int node){
	bool exists = true;
	
	if(!data) exists= findIfNodeExistsInNetwork(keyID);
	
	pastryNode* nearestNode = NULL;
	
	if(!exists && !data) return false;
	
	if(node != -1){
		int i = 0;
		for(auto it = mapIdNode.begin(); it != mapIdNode.end(); it++){
			if(i == node){
			 	nearestNode = it -> second;
			 	break;
			 }
			i++;
		}
		
	}else{
		
		map<string, pastryNode*>:: iterator it = mapIdNode.begin();
		pastryNode* nearestNode = it -> second;
		
	}
	
	if(!nearestNode){
		cout<<"There are no nodes in network";
		return false;
	}
	
	vector<pastryNode*> pathInRoute;
	nearestNode -> route(keyID, pathInRoute);
	cout<<"Route: ";
	
	if(printRoute){
		for(int i = 0; i < pathInRoute.size()-1; i++){
			cout<<pathInRoute[i] -> getNodeID();
			cout<<"->";
	}
		cout<<pathInRoute[pathInRoute.size()-1] -> getNodeID()<<endl;
		
	}
	
	if(data){
	
		string dataGot = pathInRoute[pathInRoute.size()-1] -> getData(keyID);
		
		hopsGetData.push_back(pathInRoute.size()-1);
		cout<<"The corresponding data is: "<< dataGot<<" and size of path is: "<<pathInRoute.size()-1<<endl;
	}
	return true;
	
	
}

bool Pastry::setDataOnAppNode(string data, bool senderSet, string senderID){
	string key = md5(data);
	pastryNode* nearestNode = NULL;
	
	if(!senderSet && senderID.size()>0){
		int randNodePos = stoi(senderID);
		
		int i = 0;
		for(auto it = mapIdNode.begin(); it != mapIdNode.end(); it++){
			if(i == randNodePos){
			 	nearestNode = it -> second;
			 	break;
			 }	
			i++;
		}
		
	}else if(!senderSet && senderID.size() == 0){
		map<string, pastryNode*>::iterator it = mapIdNode.begin();
		nearestNode = it -> second;
	}else{
		map<string, pastryNode*>::iterator it = mapIdNode.find(senderID);
		nearestNode = it -> second;
	}
	
	if(!nearestNode){
		cout<<"There are no nodes in network";
		return false;
	}
	
	vector<pastryNode*> pathInRoute;
	nearestNode -> route(key, pathInRoute);
	
	for(int i = 0; i < pathInRoute.size()-1; i++){
			cout<<pathInRoute[i] -> getNodeID();
			cout<<"->";
	}
	if(pathInRoute.size()) cout<<pathInRoute[pathInRoute.size()-1] -> getNodeID()<<endl;
		
	hopsSetData.push_back(pathInRoute.size()-1);
	pathInRoute[pathInRoute.size()-1] -> setData(key, data);
}


pastryNode* Pastry::initializeAGivenNode(){
	int ipAddress[4];
	int gpsLocation[2];
	cout<<"Enter IP Address Octets:"<<endl;
	cin>>ipAddress[3];
	cin>>ipAddress[2];
	cin>>ipAddress[1];
	cin>>ipAddress[0];
	if(isIPpresent(ipAddress[3], ipAddress[2], ipAddress[1], ipAddress[0], numOfNodes))return NULL;
	
	cout<<"Enter the latitude and logitude coordinates: "<<endl;
	cin>>gpsLocation[0];
	cin>>gpsLocation[1];
	if(islocationPresent(gpsLocation[0], gpsLocation[1], numOfNodes)) return NULL;
	
	pastryNode* newNode = new pastryNode(row, column,ipAddress, gpsLocation, L, M);
	nodeList.push_back(newNode);
	
	string ipAdd = to_string(newNode ->getIpAddress(0));
	ipAdd += to_string(newNode -> getIpAddress(1));
	ipAdd += to_string(newNode ->getIpAddress(2));
	ipAdd += to_string(newNode ->getIpAddress(3));
			
	string nodeID = md5(ipAdd);
	//string nodeID = "ff89a8701557a0ca2a26b5dd8fa760e";
	//nodeID += to_string(i);
	//cout << "node id generated"<<i<<": "<<nodeID<<endl;
			
	newNode -> setNodeID(nodeID.substr(0,row));
	return newNode;
}
	
bool Pastry::insertNodeInPastryNetwork(pastryNode* node, bool printRoute){
		vector<pastryNode*> routingPath;
		//we check if the node is present in network
		bool exists = findIfNodeExistsInNetwork(node -> getNodeID());
		if(exists) return false;
		
		//we first get the nearest node
		pastryNode* nearestNode = getNearestGeographicalNode(node);
		
		
		if(!nearestNode){
			//cout <<"First node inserted: "<<node -> getNodeID()<<endl;
			//This is only node present in the network
			//we initialize its routing table with its one nodeId
			string currNodeId = node -> getNodeID();
			//Insert this node in MAP
			mapIdNode.insert(std::make_pair(currNodeId, node));
			//cout<<"string length: "<<nodeIDLength;
			for(int i = 0; i < nodeIDLength; i++){
				if(currNodeId[i] - '0' <= 9){
				
				 
				 node -> setRoutingTableEntry(i, currNodeId[i] - '0', currNodeId);
				}else{
				 
				 node -> setRoutingTableEntry(i, currNodeId[i] - 'a' + 10, currNodeId);	
				 }
			
			}
			
			//set neighbourhood set to itself
			node -> setNeighbourhoodSetEntry(0, currNodeId);
			
			//set its leaf set with middle element as itself
			node -> setLeafSetEntry(L/2, currNodeId);
		}else{
			
			//cout<<"subsequentNode Inserted: "<<node -> getNodeID()<<endl;
			//there are nodes already present in network then we join the network using the nearest node
			
			
			string insertNodeID = node -> getNodeID();
			nearestNode -> route(insertNodeID, routingPath);
					
			//cout<<"the size of routing path: "<<routingPath.size()<<endl;
			node -> initRoutingPath(routingPath);
			
			//add this node to the map
			mapIdNode.insert(std::make_pair(insertNodeID, node));
			//now we construct leaf set, routing table and neighbouring set of this node
			
			node -> initializeRoutingTable(row, column);
			node -> initializeLeafSet(L);
			node -> initializeNeighbourhoodSet(M);
			
			//then we update the entries of node entries we have
			for(int i = 0; i < M; i++ ){
				string nodeId = node -> getNeighbourhoodSetEntry(i);
				if(nodeId == "" || nodeId == insertNodeID) continue;
				map<string, pastryNode*>:: iterator it = mapIdNode.find(nodeId);
				pastryNode* temp = it -> second;
				temp -> updateInfoNodeInserted(node, M, L, row, column);
			}	
			
			for(int i = 0; i < L; i++ ){
				string nodeId = node -> getLeafSetEntry(i);
				if(nodeId == "" || nodeId == insertNodeID) continue;
				map<string, pastryNode*>:: iterator it = mapIdNode.find(nodeId);
				pastryNode* temp = it -> second;
				temp -> updateInfoNodeInserted(node, M , L, row, column);
			}
			
			for(int i = 0; i < row; i++ ){
				for(int j = 0 ; j < column; j++){
					string nodeId = node -> getRoutingTableEntry(i,j);
					if(nodeId == "" || nodeId == insertNodeID) continue;
					map<string, pastryNode*>:: iterator it = mapIdNode.find(nodeId);
					pastryNode* temp = it -> second;
					temp -> updateInfoNodeInserted(node, M, L, row, column);
				}
				
			}
			
			
		}		
		
		if(printRoute && routingPath.size() > 0){
			//cout<<"routing Path size: "<<routingPath.size()<<endl;
			cout<<"Route: ";
			for(int i = 0; i < routingPath.size()-1; i++){
				cout<<routingPath[i] -> getNodeID();
				cout<<"->";
			}
		
			cout<<routingPath[routingPath.size()-1] -> getNodeID()<<endl;
		}
		
		return true;
}
	
bool Pastry::deleteNodeInPastry(string nodeToBeDeleted){
		
		pastryNode* node = getActiveNodeWithID(nodeToBeDeleted);
		
		if(!node) return false;
		
		
		
		string tmp = "";
		//we first check if node to be deleted has some data and then look into leaf set to see if it has a node we can transfer data to
		if(node -> data.size() > 0 && !node -> getLeafSetEntry(L/2-1).empty()) tmp = node -> getLeafSetEntry(L/2-1);
		else if(node -> data.size() > 0 && !node -> getLeafSetEntry(L/2+1).empty()) tmp = node -> getLeafSetEntry(L/2+1);		
		
		//for all the leaf nodes update their constituent entries
		for(int i = 0; i < L; i++){
			string tmpID = node -> getLeafSetEntry(i);
			if(tmpID == "" || tmpID == nodeToBeDeleted) continue;
			map<string, pastryNode*>:: iterator it = mapIdNode.find(tmpID);
			pastryNode* tmp = it -> second;
			tmp -> updateInfoNodeDeleted(node, M, L, row, column);
			
			
		}
		
		
		for(int i = 0; i < M; i++){
			string tmpID = node -> getNeighbourhoodSetEntry(i);
			if(tmpID == "" || tmpID == nodeToBeDeleted) continue;
			map<string, pastryNode*>:: iterator it = mapIdNode.find(tmpID);
			pastryNode* tmp = it -> second;
			tmp -> updateInfoNodeDeleted(node, M, L, row, column); 
			
			//if no nearest node to transfer data was found we 
			
		}
		
		for(int i = 0; i < row; i++){
			for(int j = 0; j < column; j++){
				string nodeId = node -> getRoutingTableEntry(i, j);
				if(nodeId == "" || nodeId == nodeToBeDeleted) continue;
				map<string, pastryNode*>:: iterator it = mapIdNode.find(nodeId);
				pastryNode* tmp = it -> second;
				tmp -> updateInfoNodeDeleted(node, M, L, row, column);
			
			}
		
		}
		mapIdNode.erase(nodeToBeDeleted);
		//if no leaf node was found then we redistribute the keys by routing them
		if(node -> data.size() > 0 && tmp == ""){
			string data = "";
			for(auto it = node -> data.begin(); it != node -> data.end(); it++){
				data = it -> second;
				setDataOnAppNode(data, true, nodeToBeDeleted);
			}
		}
		
		return true;
		
	}

void Pastry::printAllCurrentNodes(){
	for(int i = 0; i < numOfNodes; i++){
		cout<<"Node "<<i<<": "<<"IP ADDRESS:"<<nodeList[i]-> getIpAddress(3)<<"."
		<<nodeList[i]-> getIpAddress(2)<<"."<< nodeList[i]-> getIpAddress(1)
		<<"."<<nodeList[i]-> getIpAddress(0)<< "   LOCATION:"<<nodeList[i] -> getGpsLocation(0)<<
		", "<<nodeList[i] -> getGpsLocation(1)<<endl;
	
	}

}
pastryNode* Pastry::getNearestGeographicalNode(pastryNode* node){
		int minDist = INT_MAX;
		pastryNode* tmp = NULL;
		int deltaLat = 0;
		int deltaLong = 0;
		int dist = minDist;
		
		map<string, pastryNode*>::iterator it;
		
		for(it = mapIdNode.begin(); it != mapIdNode.end(); it++){
			//cout<<"runs";
			deltaLat = (node -> getGpsLocation(0) - it -> second -> getGpsLocation(0));
			deltaLong = (node -> getGpsLocation(1) - it -> second -> getGpsLocation(1));
			
			if(deltaLat < 0) deltaLat = -deltaLat;
			if(deltaLong < 0) deltaLong = -deltaLong;
			
			dist = deltaLat + deltaLong;
			if(dist < minDist){ 
				dist = minDist;
				tmp = it -> second;
			}
		
		}
		return tmp;	
}
	
pastryNode* Pastry::getActiveNodeWithID(string nodeID){
		//map<string, pastryNode*> iterator it;
		
		map <string, pastryNode*>:: iterator it = Pastry::mapIdNode.find(nodeID);
		pastryNode* tmp = it -> second;
		
		if(tmp) return tmp;
		else return NULL; 
}


pastryNode::pastryNode(int row, int column, int* ipAdd, int* gpsL, int L, int M):routingTable(row, vector<string>(column, "")), neighbourhoodSet(M), leafSet(L){
		for(int i = 0; i < 4; i++){
		 	ipAddress[i] = *ipAdd;
		 	ipAdd++;
		}
		
		for(int i = 0; i < 2; i++){
		 	gpsLocation[i] = *gpsL;
		 	gpsL++;
		}
		
}
	
bool pastryNode::searchInLeafSet(string keyID, bool range){
		int L = leafSet.size();
		if(range){
		string left = "";
		string right = "";
		
		
		
		for(int i = 0; i <= L/2; i++){
			if(leafSet[i] == "")continue;
			 	left = leafSet[i];
			 	
		}
		for(int i = L-1 ; i >= L/2; i--){
			if(leafSet[i] == "")continue;
			 	right = leafSet[i];
			 	
		}
		if(left == right && left != keyID) return false;
		
		if(getIDInDecimal(left) <= getIDInDecimal(keyID) && getIDInDecimal(right) >= getIDInDecimal(keyID)) return true;
		else
		return false;}
		else{
			for(int i = 0; i < L; i++){
				if(leafSet[i] == keyID) return true;
			}
			return false;
		}
}

bool pastryNode::searchInNeighbourhoodSet(string keyID){
	for(int i= 0; i < neighbourhoodSet.size(); i++){
		if(neighbourhoodSet[i] == keyID) return true;
	}
	return false;
}
	
void pastryNode::setData(std::string key, std::string data){
	//for(auto itr = data.begin(); itr != data.end(); itr++){
		
	//}
	this -> data.insert(make_pair(key, data));
	cout<<"data: "<<key<<" inserted on node: "<< nodeID<<endl;
}	

string pastryNode::getData(string key){
		map <string, string>:: iterator it = data.find(key);
		if(it == data.end()) return "";
		else return it -> second; 
}	


void pastryNode::route(string keyId, vector<pastryNode*>& routingPath){
		
		routingPath.push_back(this);
		
		//the node we are looking for is this node
		if(keyId == this -> nodeID) return;
		
		//else we look in leaf set
		bool isInLeafSet = searchInLeafSet(keyId, true);
		
		if(isInLeafSet){
			//cout<<"Searching in leaf set"<<endl;
			string nearestLeaf = "";
			long long int minDist = diffBetweenID(keyId, nodeID);
			
			for(int i = 0; i < leafSet.size(); i++){
				if(leafSet[i] == ""&& !notInPath(leafSet[i], routingPath)) continue;
				
				string tmp = leafSet[i];
				long long int dist = diffBetweenID(tmp, keyId);
				
				if(dist < minDist || (dist == minDist && getIDInDecimal(tmp) <= getIDInDecimal(nearestLeaf))){
					minDist = dist;
					nearestLeaf = tmp;
				}
			
			}
			
			pastryNode* nearestNode = Pastry::getActiveNodeWithID(nearestLeaf);
			
			long long int myDist = diffBetweenID(keyId, nodeID);
			
			if(myDist < minDist || (myDist == minDist && getIDInDecimal(nodeID) < getIDInDecimal(nearestLeaf))) {return;
			}else{
				string a = routingPath[routingPath.size()-1]-> getNodeID();
				if(routingPath.size() >= 1 && nearestLeaf == a) return;
				else routingPath.push_back(nearestNode);
			}
			
		}else{
			//cout<<"search in other neighbouhood set"<<endl;
			//not within range of leaf set 
			// so we find the common prefix
			
			int l = getCommonPrefix(this->nodeID, keyId);
			if(l == keyId.length()){
				//the entry we got is of current node
				return ;
			}
			//Find if there is an entry in routing table where
			// the character of node to insert keyid char is present
			int j = (keyId[l] - '0' <= 9)? (keyId[l] - '0'): keyId[l] - 'a'+10;
			string nextNode = routingTable[l][j];	
			if(!nextNode.empty() && notInPath(nextNode, routingPath)) {
				//there was an entry there then we take that node as our next node 
				//cout<<"in rt "<<nodeID<<" "<<keyId<<endl;
				if(diffBetweenID(keyId, nodeID) < diffBetweenID(keyId, nextNode)){ 
				//cout<<nextNode<<" "<<diffBetweenID(keyId, nodeID)<<" "<<diffBetweenID(keyId, nextNode)<<endl;
				
				return;}
				if(nextNode == nodeID) return;
				pastryNode* nextNodeInRoute = Pastry::getActiveNodeWithID(nextNode);
				nextNodeInRoute ->route(keyId, routingPath);
			}else{
				//cout<<endl;
				//cout<<"nothing matches"<<endl;
				//we search from a union of all the nodes present in leaf set, neighbourhood set, routing table
					string bestMatchNode = "";
					long long int dist = INT_MAX; 					string nodeInserted = keyId;
					long long minDist = diffBetweenID(nodeInserted, nodeID);
					for(int i = 0; i < leafSet.size(); i++){
						if((leafSet[i] == "" ||leafSet[i] == nodeID)||!notInPath(leafSet[i], routingPath))continue;							
						
							dist = diffBetweenID(nodeInserted, leafSet[i]);	
							//cout<<leafSet[i]<<"distance between leaf set: "<<dist<<endl;
							if(dist < minDist){
								minDist = dist;
								bestMatchNode = leafSet[i];
							}
						
					}
					
				
					for(int j = 0; j < neighbourhoodSet.size(); j++){
						if(neighbourhoodSet[j] == "" || neighbourhoodSet[j] == nodeID || !notInPath(neighbourhoodSet[j], routingPath))continue;
							dist = diffBetweenID(nodeInserted, neighbourhoodSet[j]);
												//cout<<neighbourhoodSet[j]<<" distance between neighbourhood set: "<<dist<<endl;
							if(dist < minDist){
								minDist = dist;
								bestMatchNode = neighbourhoodSet[j];
							}
						}
					
					
					for(int i = 0; i < routingTable.size(); i++){
						for(int j = 0; j < routingTable[0].size(); j++){
							if(routingTable[i][j] == "" || routingTable[i][j] == nodeID || !notInPath(routingTable[i][j], routingPath)) continue;
							
							string RTEntry = routingTable[i][j];
							dist = diffBetweenID(nodeInserted, RTEntry);
											//cout<<routingTable[i][j]<<" distance between routing TAble set: "<<dist<<endl;
							if(dist < minDist){
								minDist = dist;
								bestMatchNode = RTEntry;
							}
						}
					}
				//If this the current node is the one we need				
				//cout<<"distance between : "<<diffBetweenID(nodeID, nodeInserted)<<" "<<diffBetweenID(nodeID, bestMatchNode)<<endl;
				
				if(bestMatchNode == nodeID){
					//cout<<"match";
					return;
				}
								
				pastryNode* nextNodeInRoute = Pastry::getActiveNodeWithID(bestMatchNode);				
				
				
				if(nextNodeInRoute && !notInPath(bestMatchNode, routingPath))nextNodeInRoute -> route(nextNode, routingPath);
				
			}	
			
		}
	
}

void pastryNode::initRoutingPath(vector<pastryNode*>& tmp){
		initialRoutingPath.insert(initialRoutingPath.end(), tmp.begin(), tmp.end());
}
	
long long pastryNode::diffBetweenID(string id1, string id2){
		//cout<<id1<<" "<<id2;
		
		long long x, y;
		x = 1; 
		y = 1;
		x = getIDInDecimal(id1);
		y = getIDInDecimal(id2);
		//return -1;
		return (x - y < 0)? -(x-y):(x-y); 
	
	}

long long pastryNode::getIDInDecimal(string num){
		long long i, r, len, hex = 0;
    		//len = id1.length;
    		len = 9;
    		for (i = 0; num[i] != '\0'; i++){
        		len--;
			if(num[i] >= '0' && num[i] <= '9')
				r = num[i] - 48;
			else if(num[i] >= 'a' && num[i] <= 'f')
				r = num[i] - 87;
			else if(num[i] >= 'A' && num[i] <= 'F')
			r = num[i] - 55;
			hex += r * pow(16,len);

    		}

    //cout << "\nDecimal equivalent of " << num << " is : " << hex;

    		return hex;
		/*//if(id1.length() > 8) id1 = id1.substr(0,8);
		long long x;
		long long base = 1;
		for(int i = 7; i>=0; i--){
			if(id1[i] >= '0' && id1[i] <= '9'){
				x += (id1[i] - 48)*base;
				base = base * 16;
			}else if(id1[i] >= 'a' && id1[i]<= 'f'){
				x+= (id1[i] - 97)*base;
				base = base * 16;
			}
		
		}
		
		return x;*/
}
	
void pastryNode::initializeRoutingTable(int row, int column){
		//set itself on each line
		for(int i = 0; i < nodeID.length(); i++){
			int j;
			if(nodeID[i] - '0' <= 9) j = nodeID[i] - '0';
			else j = nodeID[i] - 'a' + 10;
			routingTable[i][j] = nodeID;
		}
		
		//set the information from other nodes in the path
		//get the ith row from ith node in path
		if(initialRoutingPath.size() > 0){
			//cout<<"size of routing path: "<<initialRoutingPath.size()<<endl;
			for(int i = 0; i < initialRoutingPath.size(); i++){
				//if we get more nodes in routing path then no of nodes we
				//return
				//if(i > row) return;
				
				//cout<<"routing path elemnts: "<<initialRoutingPath[i]->getNodeID();
				pastryNode* nodeInPath = initialRoutingPath[i];
				
				
				//cout<<"size: "<<column;
				for(int j = 0; j < column; j++){
					
					string pathNodeId = nodeInPath -> getRoutingTableEntry(i, j);
					if(pathNodeId =="") continue;
				
					routingTable[i][j] = pathNodeId;
				
				
			
			}
			
		
		}
	}
}
	
void pastryNode::initializeLeafSet(int L){
		int i = L/2 -1;
		int j = L/2 + 1;
		
		//cout<<"routing path size: "<<initialRoutingPath.size()<<endl;
		pastryNode* nodeInPath = initialRoutingPath.back();
		
		leafSet[L/2] = nodeID;
		for(int k = 0; k < L; k++){
			string nodeIdtemp = nodeInPath -> getLeafSetEntry(k);
			if(nodeIdtemp == "") continue;
			
			if(getIDInDecimal(nodeIdtemp) - getIDInDecimal(this->nodeID) < 0 && i >= 0){
			 	leafSet[i] = nodeIdtemp;
			 	//cout<<"insrt1";
			 	i--;
			 }else if(getIDInDecimal(nodeIdtemp) - getIDInDecimal(this -> nodeID) > 0 && j < L){
			 	leafSet[j] = nodeIdtemp;
			 	//cout<<"insert2";
			 	j++;
			 }
		}
}
	
void pastryNode::initializeNeighbourhoodSet(int M){
				
		pastryNode* nearNode = initialRoutingPath.front();
		//cout << nearNode -> nodeID;
		for(int i = 0; i < M ; i++){
			
			neighbourhoodSet[i] = nearNode -> getNeighbourhoodSetEntry(i);
		}
	}
	
int pastryNode::geoDistBetweenNodes(pastryNode* node1, pastryNode* node2){
		int deltaLat = node1 -> getGpsLocation(0) - node2 -> getGpsLocation(0);
		if(deltaLat < 0) deltaLat = -deltaLat;
		
		int deltaLong = node1 -> getGpsLocation(1) - node2 -> getGpsLocation(1);
		if(deltaLong < 0) deltaLong = -deltaLong;
		
		return (deltaLat + deltaLong);
}
	
void pastryNode::updateInfoNodeInserted(pastryNode* nodeInserted, int M, int L, int rows, int columns){	
		//update the leaf set 
		string minHDist = "";
		bool LSupdated = false;
		string nodeInsertedID = nodeInserted -> getNodeID();
		int i;
		
		if(!searchInLeafSet(nodeInsertedID, false)){
		for(i = 0;i < L; i++ ){
			if(leafSet[i] == "" || leafSet[i] == nodeID) continue;
			
			minHDist = leafSet[i];
			
			if(diffBetweenID(nodeID, minHDist) > diffBetweenID(nodeID,nodeInsertedID)){
				leafSet[i] = nodeInsertedID;
				LSupdated = true;
				break;
			}
		
		}
		
		
		if(LSupdated){
		i++;
		
		for(; i < L-1; i++){
			
			for(int j = i + 1; j < L; j++){
				string tmp2 = leafSet[i];
				if(leafSet[i] == ""||leafSet[i] == nodeInsertedID) continue;
				if(diffBetweenID(nodeID, tmp2) > diffBetweenID(minHDist, tmp2)){
					minHDist = tmp2;
					leafSet[i] = minHDist;
				}
			}
		}
		}else{
			
			if(getIDInDecimal(nodeInsertedID) < getIDInDecimal(nodeID)){
				for(int i = L/2-1; i >= 0; i--){
					if(leafSet[i] == ""){
					 	leafSet[i] = nodeInsertedID;
					 	
					 	break;
					}
				}
			}else{
				for(int i = L/2+1; i < L; i++){
					if(leafSet[i] == ""){
					 	leafSet[i] = nodeInsertedID;
					 	
					 	break;
					 }	
				}
			}
			
		}
		}
		if(!searchInNeighbourhoodSet(nodeInsertedID)){
		//update the neighbour hood set based on distance
		pastryNode* minDistNode;
		bool NSupdated = false;
		//int i;
		for(i = 0; i < M; i++){
			
			if(neighbourhoodSet[i] == "" || neighbourhoodSet[i] == nodeID) continue;
			
			minDistNode = Pastry::getActiveNodeWithID(neighbourhoodSet[i]);
			
			if(geoDistBetweenNodes(this, nodeInserted) < geoDistBetweenNodes(nodeInserted, minDistNode)) {
			neighbourhoodSet[i] = nodeInserted -> getNodeID();
			NSupdated = true;
			break;
			}
		}
		if(NSupdated){
		//cout<<"This was ran";
		i++;
		for(; i < M-1; i++){
			for(int j = i+1; j < M; j++){
				pastryNode* tmp = Pastry::getActiveNodeWithID(neighbourhoodSet[i]);
				
				if(neighbourhoodSet[i] == "") continue;
				if(geoDistBetweenNodes(this, tmp) > geoDistBetweenNodes(this, minDistNode)){
					minDistNode = tmp;
					neighbourhoodSet[i] = minDistNode -> getNodeID();
				}
			}
		}
		}else{
			//cout<<"that was ran";
			for(int i = 0; i < M; i++){
				if(neighbourhoodSet[i] == ""|| neighbourhoodSet[i] == nodeID){
				 	neighbourhoodSet[i] = nodeInserted -> nodeID;
				 	break;
				 }
			}
		
		}
		}
		
		//update the routing table entry with node id
		string tmp1 = nodeInserted -> getNodeID();
		int row1 = getCommonPrefix(nodeID, tmp1);
		int column1 = (tmp1[row1] >= 97)? tmp1[row1] - 87 : tmp1[row1] -48;
		if(routingTable[row1][column1] == ""){
			routingTable[row1][column1] = tmp1;
		}
		
		
	}
	
	
bool Pastry::displayAllDeletedNodes(){
	if(deletedNodes.size() == 0){
		cout<<"No nodes were deleted"<<endl;
		return false;
	}
	for(int i = 0; i < deletedNodes.size(); i++){
		cout<<i+1<<": "<<deletedNodes[i] -> getNodeID()<<endl;
	}

}

pastryNode* Pastry::getDeletedNode(int i){
	 i--;
	 pastryNode* newNode = deletedNodes[i];
	 
	 for(; i < deletedNodes.size()-1; i++){
	 	deletedNodes[i] = deletedNodes[i+1];
	 }
	return newNode;
}

void pastryNode::updateInfoNodeDeleted(pastryNode* node, int M, int L, int rows, int columns){
		//update nodes in leaf set entries
		long long int a = getIDInDecimal(nodeID);
		long long int b = getIDInDecimal(node -> nodeID);
		
		if(a - b < 0){
			//the node to be deleted has larger nodeID
			int i;
			bool delNfound = false;
			for(i = L/2+1; i < L; i++){
				if(leafSet[i] == "") continue;
				if(leafSet[i] == node -> nodeID){
					delNfound = true;
					break;
				}
			}
			if(delNfound){
				int j;
				for(j = i; j < L-1; j++){
					if(leafSet[j] == "") break;
					leafSet[j] = leafSet[j+1];
				}
				j--;
				string tmp3 = leafSet[j-1];
				pastryNode* tmp = Pastry::getActiveNodeWithID(tmp3); 
				leafSet[j] = tmp -> getLeafSetEntry(L/2+1);
			
			}
		
		}else{
			int i;
			bool delNfound = false;
			
			for(i = L/2-1; i >=0; i--){
				if(leafSet[i] == "") continue;
				if(leafSet[i] == node -> nodeID){
					delNfound = true;
					break;
				}
			}
			if(delNfound){
				int j;
				for(j = i; j >= 1; j--){
					if(leafSet[j]== "") break;
					leafSet[j] = leafSet[j-1];
				}
				j++;
				
				string tmp4 = leafSet[j];
				if(tmp4 == ""){
				}else{ 
				pastryNode* tmp = Pastry::getActiveNodeWithID(tmp4);
				
				leafSet[j-1] = tmp -> getLeafSetEntry(L/2 -1);
				}
			}
			
		
		}
		
		//Now we update all the nodes in neighbourhood table and sort the table
		int i, j;
		for(i = 0; i < M; i++){
			if(neighbourhoodSet[i] == node -> nodeID){
			 	neighbourhoodSet[i] = "";
			 	break;
			 }
		}
		
		
		for(i = 0, j = 1; j < M; j++){
			if(neighbourhoodSet[i] == "" && neighbourhoodSet[j] != ""){
				neighbourhoodSet[i] = neighbourhoodSet[j];
				i++;
				neighbourhoodSet[j] = "";
			}else if(neighbourhoodSet[i] != "" && neighbourhoodSet[j] != ""){
				i++;
			}
		}
		
		
		int dist = INT_MAX; //geoDistBetweenNodes(node, this);
		int minDist = INT_MAX;
		pastryNode* minDistN = NULL;
		pastryNode* current = NULL;
		for(int j = 0; j < M; j++){
			//if(j == i) continue;
			string tmp1 = neighbourhoodSet[j];
			if(tmp1 == "") continue;
			
			pastryNode* tmpNeighbour = Pastry::getActiveNodeWithID(tmp1);
			for(int k = 0; k < M; k++){
				string tmp2 = tmpNeighbour -> getNeighbourhoodSetEntry(k);
				if(tmp2 == "" || tmp2 == nodeID) continue;
				pastryNode* tmpNNeighbour = Pastry::getActiveNodeWithID(tmp2);
				//if(!tmpNNeighbour)continue;
				pastryNode* current = Pastry::getActiveNodeWithID(nodeID);
				minDist = geoDistBetweenNodes(tmpNNeighbour, this);
				if(minDist < dist){
					dist = minDist;
					minDistN = tmpNNeighbour;
				}
			} 
			
		}
		
		if(minDistN) neighbourhoodSet[i] = minDistN -> getNodeID();
		
		
		//update the routing table of each entry in routing table
		bool foundBetter = false;
		int row1 = getCommonPrefix(nodeID, node -> nodeID);
		
		for(int j = 0; j < row; j++){
			//if the row is searched then search the next row
			if(j == columns -1){
				j = 0;
				row1 = row1 + 1;
			}
			
			if(!foundBetter){
				string nodetmp = routingTable[row1][j];
				pastryNode* nodetmp1 = Pastry::getActiveNodeWithID(nodetmp);
				int column1 = (nodetmp[row1] >=97)? nodetmp[row1] -87 : nodetmp[row1] -48;
				string nodeFromNRT = nodetmp1 -> getRoutingTableEntry(row1, column1);
				if(nodeFromNRT != node -> nodeID){
					routingTable[row1][column1] = nodeFromNRT;
					foundBetter = true;
					break;
				}
			}
			
		}
		
	
	}
	

int pastryNode::getCommonPrefix(string nodeID, string keyID){
		int length = 0;
		
		for(int i = 0; i < nodeID.length(); i++){
			if(nodeID[i] == keyID[i]) length++;
			else break;
		}
		return length;
	
}
	
void pastryNode::printRoutingTable(){
		int len = nodeID.length();
		cout<<" RoutingTable. ID: "<<nodeID<<endl;
		for(int i = 0; i < routingTable.size(); i++){
			cout<<"| ";
			for(int j = 0; j < routingTable[i].size(); j++){
				if(routingTable[i][j] == ""){
					for(int i= 0; i < 3; i++)cout<<" ";
					cout<<"|";
				}
				else cout<<routingTable[i][j][0]<<routingTable[i][j][1]<<" |";
			}
			cout<<endl;
		}
}

void pastryNode::printNeighbourhoodSet(){
	cout<<"Node: "<<nodeID<<" NeighbourhoodSet: "<<endl;
		cout<<"| ";
		for(int i = 0; i < neighbourhoodSet.size(); i++){
			if(neighbourhoodSet[i] == "")cout<<"  |";
			else cout<<neighbourhoodSet[i]<<" |";
		}
		cout<<endl;

}

void pastryNode::printLeafSet(){
	cout<<"Node: "<<nodeID<<" LeafSet: "<<endl;
		cout<<"| ";
		for(int i = 0; i < leafSet.size(); i++){
			if(leafSet[i] == "") cout<<"  |";
			else cout<<leafSet[i][0]<<leafSet[i][1]<<" |";
		}
		cout<<endl;

}
	
int pastryNode::getIpAddress(int octet){
		return ipAddress[octet];
}
	
int pastryNode::getGpsLocation(int latlon){
		//0 for latitude 1 for longitude
		return gpsLocation[latlon];
}
	
void pastryNode::setNodeID(string hash){
		nodeID = hash;
}
	
string pastryNode::getNodeID(){
		return nodeID;
}
	
void pastryNode::setRoutingTableEntry(int i, int j, string nodeID){
		//cout<<"size: "<<routingTable.size()<<" column size: "<<routingTable[i].size()<<" ";
			routingTable[i][j] = nodeID;
			
}
	
string pastryNode::getRoutingTableEntry(int i, int j){
		return routingTable[i][j];
}
	
void pastryNode::setNeighbourhoodSetEntry(int i, string nodeID){
		neighbourhoodSet[i] = nodeID;
}
	
string pastryNode::getNeighbourhoodSetEntry(int i){
		return neighbourhoodSet[i];
}
	
string pastryNode::getLeafSetEntry(int i){
		return leafSet[i];
}


void pastryNode::setLeafSetEntry(int i, string nodeID){
		leafSet[i] = nodeID;
}
	


