//
//  main.cpp
//  375 - Big project
//
//  Created by Programing Programing Prodigies on 4/8/15.
//  Copyright (c) 2015 Programing Prodigies. All rights reserved.
//  Latest Change 4/19/15 6:41 pm (Samier)
//
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
ifstream File, Vendor, Customer; //Three file initializations.
string itemtotal; //Total number of items in Catalogue string form.
int total = 0; //Total number of items in catalogue, converted to int.
struct sm
{
	string small[2]; //Position 0 is ID.  Position 1 is Number of items with that ID in this slot.
};
struct med
{
	string medium[2]; //Position 0 is ID.  Position 1 is Number of items with that ID in this slot.
};
struct lar
{
	string large[2]; //Position 0 is ID.  Position 1 is Number of items with that ID in this slot.
};
struct list //Information about each item.
{
	string itemid;
	string itemname;
	string itemsize;
	string itemprice;
	string itemdescription;
	list *next; //Will be a linked list.
};
list *head;  //Catalogue of items
struct warehouse  //Contains the slots for each size of item.
{
	sm sloc[20];//20 small slots
	med medloc[60];//60 medium slots
	lar lloc[20];//20 large slots
};
bool check(string item, list *p, int num)
{
	for (int i = 0; i<total; i++)  //Compares item id to every node.
	{
		if (p->itemid == item)
		{
			return true; //If found, return true.
		}
		p = p->next;
	}
	return false; //Otherwise, return false.
}
void setuplist(list *p)  //Intakes the catalogue and stores it locally in the program.
{
	string temp;
	//getline(File,temp); //For the first line, 'List last created on: '
	while (!File.eof())
	{
		getline(File, temp);
		if (temp.size()<10) //For last line of catalogue - T#items
		{
			if (temp.size() == 4)
			{
				itemtotal = temp.substr(1, 3); //Amount of items in catalogue.
			}
			else if (temp.size() == 3)
			{
				itemtotal = temp.substr(1, 2);
			}
			else
				itemtotal = temp.substr(1, 1);
		}
		else  //For every other line in the catalogue, sets up a node for each in a linked list.
		{
			p->itemid = temp.substr(0, 10);
			p->itemname = temp.substr(10, 30);
			p->itemsize = temp.substr(40, 1);
			p->itemprice = temp.substr(41, 8);
			p->itemdescription = temp.substr(49, 500);
			p->next = new list;
			p = p->next;
		}
	}
	total = atoi(itemtotal.c_str()); //Converts the total number from string to int.
}
void vendor(warehouse num[3])  //For when the Vendor file is ingested.
{
	string input, today, fileid, item, number, count, shipped, comp, sentnum, numven, lineitems, itemsize, tempFromForm;
	bool itemAlreadyInWarehouse = false;
	bool sizeFound = false;//A break below was messing things up, came up with a bool to track if we grabbed the size or not
	bool warehousesChecked[3];
	bool noRoom = false;
	bool doneStoring = false;
	warehousesChecked[0]= false;
	warehousesChecked[1]= false;
	warehousesChecked[2]= false;
	getline(Vendor, input);  //First line with File ID and date receieved.
	fileid = input.substr(1, 4);
	today = input.substr(6, 8); 
	//cout << "File #" << fileid << "  Shipment recieved on: " << today.substr(0, 2) << "-" << today.substr(2, 2) << "-" << today.substr(4, 4) << endl << endl;
	while (!Vendor.eof())  //Ticks once for every vendor.
	{
		getline(Vendor, input);  //Gets vendor info and amount of items.
		if (input.size()>13)  //If not last line of file, continue.
		{
			comp = input.substr(0, 50); shipped = input.substr(50, 10); sentnum = input.substr(60, 1);
			//cout <<endl<< "Shipment from: " << comp << endl << "Sent on: " << shipped << endl << "Number of items: " << sentnum << endl;
			int i = atoi(sentnum.c_str());  //Gets number of items from vendor in int form.
		
			for (int j = i; j>0; j--)
			{
				getline(Vendor, input);
				doneStoring = false;
				item = input.substr(0, 10); number = input.substr(11, 1);
				if (input.size() == 14){ count = input.substr(13, 1); }
				else if (input.size() == 15){ count = input.substr(13, 2); }
				else if (input.size() == 16){ count = input.substr(13, 3); }
				else if (input.size() == 17){ count = input.substr(13, 4); }
				int n = (atoi(number.c_str()) - 1);
				int numberFromForm = (atoi(count.c_str()));
				if (check(item, head, total) != true)
				{
					//cout << "Item " << item << " is not in the catalogue, will not store item." << endl;
				}
				else
				{
					list *temp; temp = head;
					int y = 0;
					while(y<total && sizeFound == false)//Find's item's size from catalogue
					{					
						if (temp->itemid == item)
						{
							itemsize = temp->itemsize; //Grabs Item Size from catalogue.
							sizeFound=true;//We found the size
						}
						if(sizeFound == false)//Until we find the size keep looking
						{
						temp = temp->next;
						}
						y++;
					}
					sizeFound = false;//reset to false
					bool itemAlreadyInWarehouse = false;//reset to false
						int s = 0;
						int l = 0;
						int m = 0;
					if (itemsize == "S")  //If item size is small.
					{					
						//The below chunk checks to see if the item already exists in the warehouse					
						while(s<20 && doneStoring == false)//Look at all 20 small spots in the warehouse
						{
							bool itemAlreadyInWarehouse = false;//reset to false
							if (num[n].sloc[s].small[0] == item && num[n].sloc[s].small[1]!="250")//If item is in warehouse and the slot isnt full
							{
								itemAlreadyInWarehouse = true;//Item is in warehouse
							}
							if(itemAlreadyInWarehouse == true)
							{
							tempFromForm = to_string(static_cast<long long>(numberFromForm));
								int numberAlreadyInWarehouse = (atoi(num[n].sloc[s].small[1].c_str()));
							while(numberFromForm>0 && numberAlreadyInWarehouse < 250)//While there are some to be added from the form and still room in the slot
							{
								
								numberFromForm--;//Remove one from number to be added to warehouse
								numberAlreadyInWarehouse++;//Add one to number in warehouse
							}
							if(numberFromForm == 0)//If we added every item from the vendor form
							{
								doneStoring = true;//We have stored every item from the vendor form
								num[n].sloc[s].small[1]=to_string(static_cast<long long>(numberAlreadyInWarehouse));//Convert numberAlreadyInWarehouse back into a string to be stored in num[n].sloc[i].small[1]
							//cout<<"Stored "<<tempFromForm<<" of the small item with ID: "<<num[n].sloc[s].small[0]<<" in location "<<s<<" of warehouse "<<n+1<<endl;					
							}
							if(numberAlreadyInWarehouse == 250 && numberFromForm>0)//If we filled the slot and there are still items remaining to be added
							{
								num[n].sloc[s].small[1]=to_string(static_cast<long long>(numberAlreadyInWarehouse));//Convert numberAlreadyInWarehouse back into a string to be stored in num[n].sloc[i].small[1]
						//	cout<<"Stored "<<num[n].sloc[s].small[1]<<" of the small item with ID: "<<num[n].sloc[s].small[0]<<" in location "<<s<<" of warehouse "<<n+1<<endl;
							if(s == 19){
								//cout<<"There was not enough room in warehouse "<< n+1 << " to store the " <<numberFromForm<< " items with ID " <<item<<endl;
							}
							}
							else//item perfectly fills slot
							{								
							}
							}
							else//The below chunk runs if the item does not already exist in the warehouse.
							{
						//All below: Search for an empty slot. If none exist then check another warehouse.If all are full, tell analyst.					
						for (int r = 0; r<20; r++)//Look at all 20 small spots in the warehouse
						{
							if(num[n].sloc[0].small[0] == ""){
								num[n].sloc[r].small[0] = item;//Put item ID in first empty slot
								s--;//Restart loop for first item, there is now a place to add the items to.
								break;
							}
							else if(r>0 && num[n].sloc[r].small[0] == "")
							{
								num[n].sloc[r].small[0] = item;//Put item ID in first empty slot
								s = r;
								s--;
								break;
							}
						else if(num[n].sloc[r].small[0] == "")
						{
							num[n].sloc[r].small[0] = item;//Put item ID in first empty slot
							break;
						}
						if(r==19)//checked whole warehouse, it is full
						{
							noRoom = true;
						}
						}
						if(noRoom==true)
						{
							noRoom = false;
							//Check another Warehouse
							warehousesChecked[n] = true;//set the warehouse we just checked as checked
							if(warehousesChecked[0]== false)//if we haven't checked warehouse 1, check it
							{
								n = 0;
							}
							else if(warehousesChecked[1]==false)//if we haven't checked warehouse 2, check it
							{
								n = 1;
							}
							else if(warehousesChecked[2]==false)//if we haven't checked warehouse 3, check it
							{
								n = 2;
							}
							else
							{
								//Tell analyst all warehouses are full
								//Set all warehouses to unchecked
								warehousesChecked[0]= false;
								warehousesChecked[1]= false;
								warehousesChecked[2]= false;
							}						
						}						
							}
							s++;
						}									
					}					
					else if (itemsize == "M") //If item size is medium.
					{
						//The below chunk checks to see if the item already exists in the warehouse					
						while(m<60 && doneStoring == false)//Look at all 60 medium spots in the warehouse
						{
							bool itemAlreadyInWarehouse = false;//reset to false
							if (num[n].medloc[m].medium[0] == item && num[n].medloc[m].medium[1]!="100")//If item is in warehouse and the slot isnt full
							{
								itemAlreadyInWarehouse = true;//Item is in warehouse
							}
							if(itemAlreadyInWarehouse == true)
							{
								tempFromForm = to_string(static_cast<long long>(numberFromForm));
								int numberAlreadyInWarehouse = (atoi(num[n].medloc[m].medium[1].c_str()));
							while(numberFromForm>0 && numberAlreadyInWarehouse < 100)//While there are some to be added from the form and still room in the slot
							{
								
								numberFromForm--;//Remove one from number to be added to warehouse
								numberAlreadyInWarehouse++;//Add one to number in warehouse
							}
							if(numberFromForm == 0)//If we added every item from the vendor form
							{
								doneStoring = true;//We have stored every item from the vendor form
								num[n].medloc[m].medium[1]=to_string(static_cast<long long>(numberAlreadyInWarehouse));//Convert numberAlreadyInWarehouse back into a string to be stored in num[n].sloc[i].medium[1]
							//cout<<"Stored "<<tempFromForm<<" of the medium item with ID: "<<num[n].medloc[m].medium[0]<<" in location "<<m<<" of warehouse "<<n+1<<endl;					
							}
							if(numberAlreadyInWarehouse == 100 && numberFromForm>0)//If we filled the slot and there are still items remaining to be added
							{
								num[n].medloc[m].medium[1]=to_string(static_cast<long long>(numberAlreadyInWarehouse));//Convert numberAlreadyInWarehouse back into a string to be stored in num[n].sloc[i].medium[1]
							//cout<<"Stored "<<num[n].medloc[m].medium[1]<<" of the medium item with ID: "<<num[n].medloc[m].medium[0]<<" in location "<<m<<" of warehouse "<<n+1<<endl;
							if(m == 19){
								//cout<<"There was not enough room in warehouse "<< n+1 << " to store the " <<numberFromForm<< " items with ID " <<item<<endl;
							}
							}
							else//item perfectly fills slot
							{								
							}
							}
							else//The below chunk runs if the item does not already exist in the warehouse.
							{
						//All below: Search for an empty slot. If none exist then check another warehouse.If all are full, tell analyst.					
						for (int r = 0; r<20; r++)//Look at all 20 medium spots in the warehouse
						{
							if(num[n].medloc[0].medium[0] == ""){
								num[n].medloc[r].medium[0] = item;//Put item ID in first empty slot
								m--;//Restart loop for first item, there is now a place to add the items to.
								break;
							}
							else if(r>0 && num[n].medloc[r].medium[0] == "")
							{
								num[n].medloc[r].medium[0] = item;//Put item ID in first empty slot
								m = r;
								m--;
								break;
							}
						else if(num[n].medloc[r].medium[0] == "")
						{
							num[n].medloc[r].medium[0] = item;//Put item ID in first empty slot
							break;
						}
						if(r==19)//checked whole warehouse, it is full
						{
							noRoom = true;
						}
						}
						if(noRoom==true)
						{
							noRoom = false;
							//Check another Warehouse
							warehousesChecked[n] = true;//set the warehouse we just checked as checked
							if(warehousesChecked[0]== false)//if we haven't checked warehouse 1, check it
							{
								n = 0;
							}
							else if(warehousesChecked[1]==false)//if we haven't checked warehouse 2, check it
							{
								n = 1;
							}
							else if(warehousesChecked[2]==false)//if we haven't checked warehouse 3, check it
							{
								n = 2;
							}
							else
							{
								//Tell analyst all warehouses are full
								//Set all warehouses to unchecked
								warehousesChecked[0]= false;
								warehousesChecked[1]= false;
								warehousesChecked[2]= false;
							}						
						}						
							}
							m++;
						}		
					}
					else if (itemsize == "L") //If item size is large.
					{
						//The below chunk checks to see if the item already exists in the warehouse					
						while(l<20 && doneStoring == false)//Look at all 20 large spots in the warehouse
						{
							bool itemAlreadyInWarehouse = false;//reset to false
							if (num[n].lloc[l].large[0] == item && num[n].lloc[l].large[1]!="10")//If item is in warehouse and the slot isnt full
							{
								itemAlreadyInWarehouse = true;//Item is in warehouse
							}
							if(itemAlreadyInWarehouse == true)
							{
								tempFromForm = to_string(static_cast<long long>(numberFromForm));
								int numberAlreadyInWarehouse = (atoi(num[n].lloc[l].large[1].c_str()));
							while(numberFromForm>0 && numberAlreadyInWarehouse < 10)//While there are some to be added from the form and still room in the slot
							{
								
								numberFromForm--;//Remove one from number to be added to warehouse
								numberAlreadyInWarehouse++;//Add one to number in warehouse
							}
							if(numberFromForm == 0)//If we added every item from the vendor form
							{
								doneStoring = true;//We have stored every item from the vendor form
								num[n].lloc[l].large[1]=to_string(static_cast<long long>(numberAlreadyInWarehouse));//Convert numberAlreadyInWarehouse back into a string to be stored in num[n].sloc[i].large[1]
							//cout<<"Stored "<<tempFromForm<<" of the large item with ID: "<<num[n].lloc[l].large[0]<<" in location "<<l<<" of warehouse "<<n+1<<endl;					
							}
							if(numberAlreadyInWarehouse == 10 && numberFromForm>0)//If we filled the slot and there are still items remaining to be added
							{							
								num[n].lloc[l].large[1]=to_string(static_cast<long long>(numberAlreadyInWarehouse));//Convert numberAlreadyInWarehouse back into a string to be stored in num[n].sloc[i].large[1]
							//cout<<"Stored "<<num[n].lloc[l].large[1]<<" of the large item with ID: "<<num[n].lloc[l].large[0]<<" in location "<<l<<" of warehouse "<<n+1<<endl;
							if(l == 19){
								//cout<<"There was not enough room in warehouse "<< n+1 << " to store the " <<numberFromForm<< " items with ID " <<item<<endl;
							}
							}
							else//item perfectly fills slot
							{								
							}
							}
							else//The below chunk runs if the item does not already exist in the warehouse.
							{
						//All below: Search for an empty slot. If none exist then check another warehouse.If all are full, tell analyst.					
						for (int r = 0; r<20; r++)//Look at all 20 large spots in the warehouse
						{
							if(num[n].lloc[0].large[0] == ""){
								num[n].lloc[r].large[0] = item;//Put item ID in first empty slot
								l--;//Restart loop for first item, there is now a place to add the items to.
								break;
							}
							else if(r>0 && num[n].lloc[r].large[0] == "")
							{
								num[n].lloc[r].large[0] = item;//Put item ID in first empty slot
								l = r;
								l--;
								break;
							}
						if(r==19)//checked whole warehouse, it is full
						{
							noRoom = true;
						}
						}
						if(noRoom==true)
						{
							noRoom = false;
							//Check another Warehouse
							warehousesChecked[n] = true;//set the warehouse we just checked as checked
							if(warehousesChecked[0]== false)//if we haven't checked warehouse 1, check it
							{
								n = 0;
							}
							else if(warehousesChecked[1]==false)//if we haven't checked warehouse 2, check it
							{
								n = 1;
							}
							else if(warehousesChecked[2]==false)//if we haven't checked warehouse 3, check it
							{
								n = 2;
							}
							else
							{
								//Tell analyst all warehouses are full
								//Set all warehouses to unchecked
								warehousesChecked[0]= false;
								warehousesChecked[1]= false;
								warehousesChecked[2]= false;
							}					
						}						
							}
							l++;
						}		
					}
				}
			}//end of for loop
		}//if >13
		else
		{
		/*	lineitems = input.substr(3, 1); numven = input.substr(1, 1);
			cout << "Total number of vendors: " << numven << endl << "Total number of line items: " << lineitems << endl<< endl;
	system("pause");
			for(int n = 0; n<3; n++){//Outputs the contents of all 3 warehouses, screen too small to get first 3 small slots.
		cout<<"Warehouse "<<n+1<<endl;
				for(int i = 0; i<20; i++){
					cout<<"Small "<<i<<" ID: "<<num[n].sloc[i].small[0]<<" Count: "<<num[n].sloc[i].small[1]<<endl;
				}
		//		system("pause");
				for(int i = 0; i<60; i++){
					cout<<"Medium "<<i<<" ID: "<<num[n].medloc[i].medium[0]<<" Count: "<<num[n].medloc[i].medium[1]<<endl;
				}
				for(int i = 0; i<20; i++){
					cout<<"Large "<<i<<" ID: "<<num[n].lloc[i].large[0]<<" Count: "<<num[n].lloc[i].large[1]<<endl;
				}
				if(n == 2){
					//system("pause");
				}
			}*/
		}
	}
}
void customer(warehouse num[3])
{
	string input, type, last, first, business, straddr, comma, city, state, post, country, orderdate, ordercount, custid, orderid, payment, discount, fileid, shipdate, item, number, count, customers, lineitems, itemsize;
	//what is the business variable for???????
	ofstream Invoicef;
	bool sizeFound = false;//reset to false
	bool doneFillingOrder = false;
	int custCount = 0;
	getline(Customer, input);
	fileid = input.substr(1, 4);
	shipdate = input.substr(6, 8); 
	//cout <<endl<< "File #" << fileid << "  Shipping Date: " << shipdate.substr(0, 4) << "-" << shipdate.substr(4, 2) << "-" << shipdate.substr(6, 2) << endl << endl;
	while (!Customer.eof()) 
	{
		getline(Customer, input);//Type,Last,First
		if (input.size()>13)  //If not last line of file, continue.
		{
			


			
			
			
			//All of Customer's Info
			type = input.substr(0, 1); last = input.substr(1, 30); first = input.substr(31, 30);straddr = input.substr(61, 30);comma = input.substr(91, 1);city = input.substr(92, 20);
			state = input.substr(112, 20);post = input.substr(132, 10);country = input.substr(142, 40);orderdate = input.substr(182, 8);ordercount = input.substr(190, 1);			
	getline(Customer, input);
			custid = input.substr(0, 10);
			orderid = input.substr(10, 10);
			payment = input.substr(20, 10);
			discount = input.substr(30, 2);		
		int i = atoi(ordercount.c_str());  //Gets number of items from Customer in int form.
		custCount++;	

		   
			double overallSubtotal = 0;
		//Invoice 
			
			Invoicef.open("Invoice.txt", std::ofstream::trunc | std::ofstream::app);
			cout<<endl<<first<<" "<<last<<endl;
			cout<<straddr<<endl;
			cout<<city<<" "<<state<<" "<<post; if(country != "USA                                     " || "United States of America                          "){cout<<country;}
			cout<<endl;
			cout<<"Customer ID: " <<custid<<endl;
			cout<<"Order ID: " <<orderid<<endl;
			cout<<"Order Date: "<< orderdate.substr(4, 2)  << "-" << orderdate.substr(6, 2) << "-" << orderdate.substr(0, 4) << endl;
			cout<<"Shipping Date: "<<shipdate.substr(4, 2)  << "-" <<shipdate.substr(6, 2) << "-" <<shipdate.substr(0, 4) << endl;
			cout<<"Payment Type: "<<payment<<endl<<endl;
			cout<<"Item ID      Item Name                 Quantity     Price          Item total"<<endl;
			Invoicef<<endl<<first<<" "<<last<<endl;
			Invoicef<<straddr<<endl;
			Invoicef<<city<<" "<<state<<" "<<post; if(country != "USA                                     " || "United States of America                          "){cout<<country;}
			Invoicef<<endl;
			Invoicef<<"Customer ID: " <<custid<<endl;
			Invoicef<<"Order ID: " <<orderid<<endl;
			Invoicef<<"Order Date: "<< orderdate.substr(4, 2)  << "-" << orderdate.substr(6, 2) << "-" << orderdate.substr(0, 4) << endl;
			Invoicef<<"Shipping Date: "<<shipdate.substr(4, 2)  << "-" <<shipdate.substr(6, 2) << "-" <<shipdate.substr(0, 4) << endl;
			Invoicef<<"Payment Type: "<<payment<<endl<<endl;
		    Invoicef<<"Item ID      Item Name                 Quantity     Price          Item total"<<endl;
			
		for (int j = i; j>0; j--)
			{
				doneFillingOrder = false;
				getline(Customer, input);
				item = input.substr(0, 10); number = input.substr(11, 1);
				if (input.size() == 14){ count = input.substr(13, 1); }
				else if (input.size() == 15){ count = input.substr(13, 2); }
				else if (input.size() == 16){ count = input.substr(13, 3); }
				else if (input.size() == 17){ count = input.substr(13, 4); }
				int n = (atoi(number.c_str()) - 1);
				int numberStillNeeded = (atoi(count.c_str()));			
		if (check(item, head, total) != true)
				{
					cout << "Item " << item << " is not in the catalogue, will not store item." << endl;
					Invoicef << "Item " << item << " is not in the catalogue, will not store item." << endl;
				}
				else
				{
					list *temp; temp = head;
					int y = 0;
					while(y<total && sizeFound == false)//Find's item's size from catalogue
					{					
						if (temp->itemid == item)
						{
							itemsize = temp->itemsize; //Grabs Item Size from catalogue.
							sizeFound=true;//We found the size
								cout<<temp->itemid<<"  ";
								cout<<temp->itemname;
								Invoicef<<temp->itemid<<"  ";
								Invoicef<<temp->itemname;
						}
						if(sizeFound == false)//Until we find the size keep looking
						{
						temp = temp->next;
						}
						y++;
					}
					sizeFound = false;//reset to false
					if (itemsize == "S")  //If item size is small.
					{
int smallQuantityForInvoice = 0;
int smallGivenToCustomer = 0;
//The below chunk checks to see if the item exists in the warehouse
						int s = 19;
						while(s>=0 && doneFillingOrder == false)//Look at all 20 small spots in the warehouse
						{
							smallGivenToCustomer = 0;//reset to 0
							bool itemStillInWarehouse = false;//reset to false
							if (num[n].sloc[s].small[0] == item && num[n].sloc[s].small[1]!="0")//If item is in warehouse and the slot isnt empty
							{
								itemStillInWarehouse = true;//Item is in warehouse
							}
							if(itemStillInWarehouse == true)
							{
								int numberAlreadyInWarehouse = (atoi(num[n].sloc[s].small[1].c_str()));
							while(numberStillNeeded>0 && numberAlreadyInWarehouse > 0)//While there are some still needed by the customer and there are some in the warehouse
							{
								numberStillNeeded--;//Remove one from number needed by the customer
								numberAlreadyInWarehouse--;//Remove one from number in warehouse
								smallGivenToCustomer++;//Add one to the number given to the customer
								smallQuantityForInvoice++;
							}
							if(numberStillNeeded == 0)//If we gave the customer everything they ordered
							{
								doneFillingOrder = true;//We have given the customer all we have
								//Invoice Output Below
								cout<<smallQuantityForInvoice;
								cout<<"          $ "<<temp->itemprice;
								Invoicef<<smallQuantityForInvoice;
								Invoicef<<"          $ "<<temp->itemprice;
								string itemPriceHolder = temp->itemprice;
								double priceHolder = smallQuantityForInvoice * (stod(itemPriceHolder.c_str()));
								cout<<"      $ "<<priceHolder<<endl;
								Invoicef<<"      $ "<<priceHolder<<endl;
								overallSubtotal+=priceHolder;


								num[n].sloc[s].small[1]=to_string(static_cast<long long>(numberAlreadyInWarehouse));//Convert numberAlreadyInWarehouse back into a string to be stored in num[n].sloc[s].small[1]
								
						//	cout<<"Gave "<<smallGivenToCustomer<<" of the small item with ID: "<<num[n].sloc[s].small[0]<<" from location "<<s<<" of warehouse "<<n+1<<endl;
		if(num[n].sloc[s].small[1] == "0")//Delete item in that slot if we empty slot
								{
									num[n].sloc[s].small[0] = "";
									num[n].sloc[s].small[1] = "";
								}
							}
							else if(numberAlreadyInWarehouse == 0 && numberStillNeeded>0)//If we empty the slot and there are still items needed by the customer
							{
								smallQuantityForInvoice+=smallGivenToCustomer;
num[n].sloc[s].small[1]=to_string(static_cast<long long>(numberAlreadyInWarehouse));//Convert numberAlreadyInWarehouse back into a string to be stored in num[n].sloc[i].small[1]
							//		cout<<"Gave "<<smallGivenToCustomer<<" of the small item with ID: "<<num[n].sloc[s].small[0]<<" from location "<<s<<" of warehouse "<<n+1<<endl;
									if(num[n].sloc[s].small[1] == "0")//Delete item in that slot if we empty slot
								{
									num[n].sloc[s].small[0] = "";
									num[n].sloc[s].small[1] = "";
								}
							}
							else//Items in warehouse = items needed by customer
							{
								
							}
							}
							if(s == 0 && smallGivenToCustomer == 0){//This needs to move to next item in warehouse or check other warehouses or halt for analyst
							//	cout<<"None of item "<<item<<" is in warehouse " << n+1<<endl;
								doneFillingOrder = true;
							}
							s--;
						}
					}
					else if (itemsize == "M") //If item size is medium.
					{
												int mediumGivenToCustomer = 0;
												int mediumQuantityForInvoice = 0;
//The below chunk checks to see if the item exists in the warehouse
						int m = 19;
						while(m>=0 && doneFillingOrder == false)//Look at all 20 medium spots in the warehouse
						{
							mediumGivenToCustomer = 0;//reset to 0
							bool itemStillInWarehouse = false;//reset to false
							if (num[n].medloc[m].medium[0] == item && num[n].medloc[m].medium[1]!="0")//If item is in warehouse and the slot isnt empty
							{
								itemStillInWarehouse = true;//Item is in warehouse
							}
							if(itemStillInWarehouse == true)
							{
								int numberAlreadyInWarehouse = (atoi(num[n].medloc[m].medium[1].c_str()));
							while(numberStillNeeded>0 && numberAlreadyInWarehouse > 0)//While there are some still needed by the customer and there are some in the warehouse
							{
								numberStillNeeded--;//Remove one from number needed by the customer
								numberAlreadyInWarehouse--;//Remove one from number in warehouse
								mediumGivenToCustomer++;//Add one to the number given to the customer
								mediumQuantityForInvoice++;
							}
							if(numberStillNeeded == 0)//If we gave the customer everything they ordered
							{
								doneFillingOrder = true;//We have given the customer all we have
								//Invoice Output Below
								
								cout<<mediumQuantityForInvoice;
								cout<<"          $ "<<temp->itemprice;
								Invoicef<<mediumQuantityForInvoice;
								Invoicef<<"          $ "<<temp->itemprice;
								string itemPriceHolder = temp->itemprice;
								double priceHolder = mediumQuantityForInvoice * (stod(itemPriceHolder.c_str()));
								cout<<"      $ "<<priceHolder<<endl;
								Invoicef<<"      $ "<<priceHolder<<endl;
								overallSubtotal+=priceHolder;
	num[n].medloc[m].medium[1]=to_string(static_cast<long long>(numberAlreadyInWarehouse));//Convert numberAlreadyInWarehouse back into a string to be stored in num[n].lloc[i].medium[1]
								
							//cout<<"Gave "<<mediumGivenToCustomer<<" of the medium item with ID: "<<num[n].medloc[m].medium[0]<<" from location "<<m<<" of warehouse "<<n+1<<endl;
						if(num[n].medloc[m].medium[1] == "0")//Delete item in that slot if we empty slot
								{
									num[n].medloc[m].medium[0] = "";
									num[n].medloc[m].medium[1] = "";
								}
							}
							else if(numberAlreadyInWarehouse == 0 && numberStillNeeded>0)//If we empty the slot and there are still items needed by the customer
							{
								num[n].medloc[m].medium[1]=to_string(static_cast<long long>(numberAlreadyInWarehouse));//Convert numberAlreadyInWarehouse back into a string to be stored in num[n].lloc[i].medium[1]
								//	cout<<"Gave "<<mediumGivenToCustomer<<" of the medium item with ID: "<<num[n].medloc[m].medium[0]<<" from location "<<m<<" of warehouse "<<n+1<<endl;
									if(num[n].medloc[m].medium[1] == "0")//Delete item in that slot if we empty slot
								{
									num[n].medloc[m].medium[0] = "";
									num[n].medloc[m].medium[1] = "";
								}
							}
							else//Items in warehouse = items needed by customer
							{
								
							}
							}
							if(m == 0 && mediumGivenToCustomer == 0){//This needs to move to next item in warehouse or check other warehouses or halt for analyst
							//	cout<<"None of item "<<item<<" is in warehouse " << n+1<<endl;
								doneFillingOrder = true;
							}
							m--;
						}
					}
					else if (itemsize == "L") //If item size is large.
					{
						int largeGivenToCustomer = 0;
						int largeQuantityForInvoice = 0;
//The below chunk checks to see if the item exists in the warehouse
						int l = 19;
						while(l>=0 && doneFillingOrder == false)//Look at all 20 large spots in the warehouse
						{
							largeGivenToCustomer = 0;//reset to 0
							bool itemStillInWarehouse = false;//reset to false
							if (num[n].lloc[l].large[0] == item && num[n].lloc[l].large[1]!="0")//If item is in warehouse and the slot isnt empty
							{
								itemStillInWarehouse = true;//Item is in warehouse
							}
							if(itemStillInWarehouse == true)
							{
								int numberAlreadyInWarehouse = (atoi(num[n].lloc[l].large[1].c_str()));
							while(numberStillNeeded>0 && numberAlreadyInWarehouse > 0)//While there are some still needed by the customer and there are some in the warehouse
							{
								numberStillNeeded--;//Remove one from number needed by the customer
								numberAlreadyInWarehouse--;//Remove one from number in warehouse
								largeGivenToCustomer++;//Add one to the number given to the customer
								largeQuantityForInvoice++;
							}
							if(numberStillNeeded == 0)//If we gave the customer everything they ordered
							{
								doneFillingOrder = true;//We have given the customer all we have
								//Invoice Output Below
								cout<<largeQuantityForInvoice;
								cout<<"          $ "<<temp->itemprice;
								Invoicef<<largeQuantityForInvoice;
								Invoicef<<"          $ "<<temp->itemprice;
								string itemPriceHolder = temp->itemprice;
								double priceHolder = largeQuantityForInvoice * (stod(itemPriceHolder.c_str()));
								cout<<"      $ "<<priceHolder<<endl;
								Invoicef<<"      $ "<<priceHolder<<endl;
								overallSubtotal+=priceHolder;
									num[n].lloc[l].large[1]=to_string(static_cast<long long>(numberAlreadyInWarehouse));//Convert numberAlreadyInWarehouse back into a string to be stored in num[n].lloc[i].large[1]
						//	cout<<"Gave "<<largeGivenToCustomer<<" of the Large item with ID: "<<num[n].lloc[l].large[0]<<" from location "<<l<<" of warehouse "<<n+1<<endl;
						if(num[n].lloc[l].large[1] == "0")//Delete item in that slot if we empty slot
								{
									num[n].lloc[l].large[0] = "";
									num[n].lloc[l].large[1] = "";
								}
							}
							else if(numberAlreadyInWarehouse == 0 && numberStillNeeded>0)//If we empty the slot and there are still items needed by the customer
							{
								num[n].lloc[l].large[1]=to_string(static_cast<long long>(numberAlreadyInWarehouse));//Convert numberAlreadyInWarehouse back into a string to be stored in num[n].lloc[i].large[1]
							//		cout<<"Gave "<<largeGivenToCustomer<<" of the Large item with ID: "<<num[n].lloc[l].large[0]<<" from location "<<l<<" of warehouse "<<n+1<<endl;
									if(num[n].lloc[l].large[1] == "0")//Delete item in that slot if we empty slot
								{
									num[n].lloc[l].large[0] = "";
									num[n].lloc[l].large[1] = "";
								}
							}
							else//Items in warehouse = items needed by customer
							{
								
							}
							}
							if(l == 0 && largeGivenToCustomer == 0){//This needs to move to next item in warehouse or check other warehouses or halt for analyst
							//	cout<<"None of item "<<item<<" is in warehouse " << n+1<<endl;
								doneFillingOrder = true;
							}
							l--;
						}
					}//end Large
}	
//Math For Invoice
if(j == 1){
			cout<<endl<<"Subtotal "<<"\t\t\t\t\t\t\t\t"<<overallSubtotal<<endl;	
			Invoicef<<endl<<"Subtotal "<<"\t\t\t\t\t\t\t\t"<<overallSubtotal<<endl;	
			cout<<"         "<<"\t\t\t\t\t\t\t"<<"=============="<<endl;	
			Invoicef<<"         "<<"\t\t\t\t\t\t\t"<<"=============="<<endl;	
			Invoicef<<"Discount Percentage --"<<discount<<"\t\t\t\t\t\t"<<overallSubtotal * ((stod(discount.c_str()))/100)<<endl;
			cout<<"Discount Percentage --"<<discount<<"\t\t\t\t\t\t"<<overallSubtotal * ((stod(discount.c_str()))/100)<<endl;	
			double discountedPrice = (overallSubtotal - overallSubtotal * ((stod(discount.c_str()))/100));
			cout<<"Order Total "<<"\t\t\t\t\t\t\t\t"<<discountedPrice<<endl;	
			cout<<"         "<<"\t\t\t\t\t\t\t"<<"=============="<<endl;	
			cout<<"Tax      6%"<<"\t\t\t\t\t\t\t\t"<<(discountedPrice * .06)<<endl;
			cout<<"Amount Due in "<<payment<<"\t\t\t\t\t\t"<<discountedPrice   +((overallSubtotal * ((stod(discount.c_str()))/100)) * .06)<<endl;
			Invoicef<<"Order Total "<<"\t\t\t\t\t\t\t\t"<<discountedPrice<<endl;	
			Invoicef<<"         "<<"\t\t\t\t\t\t\t"<<"=============="<<endl;	
			Invoicef<<"Tax      6%"<<"\t\t\t\t\t\t\t\t"<<(discountedPrice * .06)<<endl;
			Invoicef<<"Amount Due in "<<payment<<"\t\t\t\t\t\t"<<discountedPrice   +((overallSubtotal * ((stod(discount.c_str()))/100)) * .06)<<endl;
		  }
}//for loop end


			



		}//if >13 close
		else
		{
			//Invoicef.close();
			/*lineitems = input.substr(3, 1); customers = input.substr(1, 1);
			cout << "Total number of customers: " << customers << endl << "Total number of line items: " << lineitems << endl<< endl;	
			system("pause");
				for(int n = 0; n<3; n++){//Outputs the contents of all 3 warehouses, screen too small to get first 3 small slots.
		cout<<"Warehouse "<<n+1<<endl;
				for(int i = 0; i<20; i++){
					cout<<"Small "<<i<<" ID: "<<num[n].sloc[i].small[0]<<" Count: "<<num[n].sloc[i].small[1]<<endl;
				}
				//system("pause");
				for(int i = 0; i<60; i++){
					cout<<"Medium "<<i<<" ID: "<<num[n].medloc[i].medium[0]<<" Count: "<<num[n].medloc[i].medium[1]<<endl;
				}
				for(int i = 0; i<20; i++){
					cout<<"Large "<<i<<" ID: "<<num[n].lloc[i].large[0]<<" Count: "<<num[n].lloc[i].large[1]<<endl;
				}
				if(n == 2){
					//system("pause");
				}
				
			}*/
		}
	}			
}
void search(warehouse num[3], string itemID)
{
	bool found = false; 
	for(int n = 0; n<3; n++){//Outputs the contents of all 3 warehouses, screen too small to get first 3 small slots.
				for(int i = 0; i<20; i++){
					if(num[n].sloc[i].small[0] == itemID)
					{
						cout << "Found at warehouse " << n + 1 << " location " << i << " in quantity " << num[n].sloc[i].small[1] << endl;
					}
				}

				for(int i = 0; i<60; i++){
					if(num[n].medloc[i].medium[0] == itemID)
					{
						cout << "Found at warehouse " << n + 1<< " location " << i << " in quantity " << num[n].medloc[i].medium[1] << endl;
					}

				}
				for(int i = 0; i<20; i++){

					if(num[n].lloc[i].large[0] == itemID)
					{
						cout << "Found at warehouse " << n + 1<< " location " << i << " in quantity " << num[n].lloc[i].large[1] << endl;
					}
					
				}

			}
}
void saveProgress(warehouse num[3])
{
	ofstream save("Progress.sav"); //Creates the save file for what is stored in warehouse
	for (int n = 0; n < 3; n++) //Writes what is in the warehouse.
	{
		save << "Warehouse " << n + 1 << endl;
		for (int i = 0; i < 20; i++){
			save << "Small " << i << " ID: " << num[n].sloc[i].small[0] << " Count: " << num[n].sloc[i].small[1] << endl;

		}
		for (int i = 0; i < 60; i++){
			save << "Medium " << i << " ID: " << num[n].medloc[i].medium[0] << " Count: " << num[n].medloc[i].medium[1] << endl;

		}
		for (int i = 0; i < 20; i++){
			save << "Large " << i << " ID: " << num[n].lloc[i].large[0] << " Count: " << num[n].lloc[i].large[1] << endl;

		}
	}

}
int main()
{
	string cataFile;
	string lookupId;
	cout<<"Enter name of Catalogue File "<<endl;
	while (true)
    {
    getline( cin, cataFile );
    File.open( cataFile.c_str() );
    if (File) break;
    cout << "Invalid file name. Please enter a valid Catalogue file name: ";
    }
	warehouse num[3];  //Three warehouse locations
	char userInput; 
	string temp;
	string vendFile, custFile;
	head = new list;
	int dayCount = 1;
	setuplist(head);
	cout<<"Catalogue Created"<<endl;
		cout<<"Enter name of Vendor File For Day "<< dayCount<<". "<<endl;
		while(true)
	{
	    getline( cin, vendFile );
		Vendor.open(vendFile.c_str());
		if (Vendor) break;
        cout << "Invalid file name. Please enter a valid Vendor file name: ";
    }
		vendor(num);
		cout<<"Enter name of Customer File For Day "<< dayCount<<". "<<endl;
		while(true)
		{
			getline(cin,custFile);
			Customer.open(custFile.c_str());
			if(Customer) break;
			cout << "Invalid file name. Please enter a valid Customer file name: ";
		}
		customer(num);
	do {
		cout<<endl<<"Please select and action below "<<endl;
		cout<<"1. Next Day."<<endl<<"2. Lookup Item Info."<<endl<<"3. Save and Quit. "<<endl<<"4. Abort without saving "<<endl;
	cin>>userInput;
	
	if(userInput == '1')
	{
		dayCount++;	
		while(true)
	{
		Vendor.close();
        Vendor.clear();
	    getline( cin, vendFile );
		Vendor.open(vendFile.c_str());
		if (Vendor) break;
        cout << "Invalid file name. Please enter a valid Vendor file name: ";
    }
		vendor(num);
		cout<<"Enter name of Customer File For Day "<< dayCount<<". "<<endl;
		while(true)
		{
			Customer.close();
			Customer.clear();
			getline(cin,custFile);
			Customer.open(custFile.c_str());
			if(Customer) break;
			cout << "Invalid file name. Please enter a valid Customer file name: ";
		}
		customer(num);
	}
	else if(userInput== '2')
	{
		cout<<"Please enter the ID of the item you want information about "<<endl;
		cin>>lookupId;
		search(num, lookupId);
	}
	else if(userInput== '3')
	{
		saveProgress(num);
		cout << "Items and progress has been saved." << endl;
		system("pause");
		return 0;
	}
	else if(userInput== '4')
	{
		abort();
	}
	else
	{
	cout<<"Not a valid entry. Retype choice.";
	}
	} while(userInput != '4');
	system("pause");
	return 0;
}