#include <iostream>
#include <bitset>
#include <stdio.h>
#include <fstream>

using namespace std;


ofstream fout("out1.txt");

class PageTableEntry
{
public:
    int virtualPageNo;
    int physicalPageNo;
    int reference;
    //unsigned char counter;
    //u_int8_t counter;
    bitset<8> counter;


    PageTableEntry()
    {
        virtualPageNo=-1;
        physicalPageNo=-1;
        reference=0;
        //counter=0;
    }
    /*
    PageTableEntry(int vpn,int ppn,int ref, u_int8_t cnt)
    {
        virtualPageNo=vpn;
        physicalPageNo=ppn;
        reference=ref;
        counter=cnt;
    }*/

};

class PageTable
{
public:
    PageTableEntry table[4];

    int nextFreePage()
    {
        for (int i = 0; i < 4; ++i)
        {
            if(table[i].physicalPageNo==-1)
                return i;
        }
        return -1;
    }

    int find(int vpn)
    {
        for (int i = 0; i < 4 ; ++i)
        {
            if(table[i].virtualPageNo==vpn)
            {
                table[i].reference=1;
                return -99;
            }

        }
        return nextFreePage();
    }

    int nfu()
    {
        unsigned char minValue=255;
        int minIndex=9;
        for (int i = 0; i < 4 ; ++i)
        {

            if((table[i].counter).to_ulong()<minValue)
            {
                minValue=(table[i].counter).to_ulong();
                minIndex=i;
            }

        }
        return minIndex;
    }

    void alloc(int vpn)
    {
        int ppn=find(vpn);
        if(ppn==-99)
            return;
        if(ppn==-1)
        {
            ppn = nfu();
        }
        table[ppn].virtualPageNo=vpn;
        table[ppn].physicalPageNo=ppn;
        table[ppn].reference=1;
        table[ppn].counter=0;

    }

    void print()
    {
        for (int i = 0; i < 4 ; ++i)
        {
            fout<<"-----vpn-----"<<"-----ppn-----"<<"-----ref-----"<<"-----cnt-----"<<endl;
            fout<<"----("<<table[i].virtualPageNo<<")----";
            fout<<"----("<<table[i].physicalPageNo<<")----";
            fout<<"----("<<table[i].reference<<")----";
            fout<<"----("<<table[i].counter.to_string()<<")----"<<endl;
            //printf("----%d----\n",table[i].counter);

        }
    }
    void interuptOperation()
    {

        for (int i = 0; i < 4 ; ++i)
        {
            table[i].counter>>=1;
            if(table[i].reference==1)
            {
                table[i].reference=0;
                //table[i].counter=table[i].counter|128;
                table[i].counter.set(7,1);
            }
        }
    }

};

int main() {
    fout << "Not Frequently Used Algorithm" << endl;
    PageTable pageTable;
    int numberOfpages;
    ifstream fin("in1.txt");
	
	if(!fin.is_open())
	{
		cout<<"Input file not found"<<endl;
		return 0;	
	}

    //cin>>numberOfpages;
    fin>>numberOfpages;
    fout<<"Number of Pages: "<<numberOfpages<<endl;
    int vpn;
    int timer=1;
    for (int i = 0; i < numberOfpages ; i++)
    {
        //cin>>vpn;
        fin>>vpn;
        fout<<"Virtual Page No: "<<vpn<<endl;
        pageTable.alloc(vpn);
        pageTable.print();
        if(timer%3==0)
        {
            fout<<"interupt occured"<<endl;
            pageTable.interuptOperation();
            pageTable.print();
        }
        timer++;
    }
    return 0;
}
