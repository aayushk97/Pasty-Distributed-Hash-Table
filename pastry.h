#include <bits/stdc++.h>
#include<vector>
#include <string>
#include <ctime>
#include <map>
#include <limits>
#include "md5.h"


using namespace std;
class pastryNode{
	string nodeID;
	int row, column;
	int ipAddress[4];
	int gpsLocation[2];
	
	vector<string> leafSet;
	vector<string> neighbourhoodSet;
	vector <vector<string>> routingTable;
			
	vector<pastryNode*> initialRoutingPath;
	
	
	//find ip address how to implement
		
	//find what is stored in table
	public:
	map<string, string> data;
	pastryNode(int row, int column, int* ipAdd, int* gpsL, int L, int M);
	
	
	void setData(string key, string data);
	bool searchInLeafSet(string keyID, bool range);
	bool searchInNeighbourhoodSet(string keyID);
	string getData(string key);
	void route(string keyId, vector<pastryNode*>& routingPath);
	
	void initRoutingPath(vector<pastryNode*>& tmp);
	
	bool notInPath(string nodeID, vector<pastryNode*>& route);
	
	long long diffBetweenID(string id1, string id2);
	
	long long getIDInDecimal(string id);
	
	void initializeRoutingTable(int row, int column);
	
	void initializeLeafSet(int L);
	
	void initializeNeighbourhoodSet(int M);
	
	int geoDistBetweenNodes(pastryNode* node1, pastryNode* node2);
	
	void updateInfoNodeInserted(pastryNode* nodeInserted, int M, int L, int rows, int columns);
	
	void updateInfoNodeDeleted(pastryNode* node, int M, int L, int rows, int columns);
	
	int getCommonPrefix(string nodeID, string keyID);
	
	int getIpAddress(int octet);
	
	int getGpsLocation(int latlon);
	
	void setNodeID(string hash);
	
	string getNodeID();
	
	void setRoutingTableEntry(int i, int j, string nodeID);
	
	string getRoutingTableEntry(int i, int j);
	
	void setNeighbourhoodSetEntry(int i, string nodeID);
	
	string getNeighbourhoodSetEntry(int i);
	
	string getLeafSetEntry(int i);
	
	void setLeafSetEntry(int i, string nodeID);
	
	void printNeighbourhoodSet();
	
	void printLeafSet();
	
	void printRoutingTable();
	
};


class Pastry{
		private:
		
		
		int bval;
		int L;
		int M;
		int row;
		int column;
		
		
	public:
		int numOfSearchQueries;
		int numOfAddQueries;
		int numOfDeleteQueries;
		int numOfDataAddQueries;
		vector<int> hopsSetData;
		vector<int> hopsGetData;
		vector<pastryNode*> nodeList;
		vector<pastryNode*> deletedNodes;
		int numOfNodes;
		int nodeIDLength;
		static map<string, pastryNode*> mapIdNode;
		
	Pastry(int n, int b, int L, int M);
	pastryNode* generateNode();
	pastryNode* initializeAGivenNode();
	pastryNode* getDeletedNode(int i);
	
	bool lookUpInPastryNetwork(string keyID, bool printRoute, bool data, int nodePos);
	
	bool displayAllDeletedNodes();
	
	bool isIPpresent(int ipOctet1, int ipOctet2, int ipOctet3, int ipOctet4, int n);
	
	bool islocationPresent(int lat, int lon, int n);
	
	bool initializePastry();
	
	
	bool findIfNodeExistsInNetwork(string node);
	
	bool insertNodeInPastryNetwork(pastryNode* node, bool printRoute);
	
	bool deleteNodeInPastry(string nodeToBeDeleted);
	
	pastryNode* getNearestGeographicalNode(pastryNode* node);
	
	static pastryNode* getActiveNodeWithID(string nodeID);

	void printAllCurrentNodes();

	//bool lookUpInPastryNetwork(string nodeID, bool printRoute);
	
	bool setDataOnAppNode(string data, bool senderSet, string sender);
	
};
