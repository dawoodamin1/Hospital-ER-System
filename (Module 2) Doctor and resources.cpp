#include "AllocationQueue.h"
#include "Doctor.h"
#include "Resource.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
using namespace std;

/*==================== QUEUE IMPLEMENTATION ====================*/

AllocationQueue::AllocationQueue() {
    front = rear = nullptr;
}

bool AllocationQueue::isEmpty() {
    return front == nullptr;
}

Node* AllocationQueue::getFront(){
    return front;
}

void AllocationQueue::enqueue(Patient p, Doctor d, Resource r) {

    Node* n = new Node{p,d,r,nullptr};

    if(isEmpty()) front = rear = n;
    else { rear->next = n; rear = n; }

    cout << "Allocated -> Patient: " << p.name
         << " | Doctor: " << d.name
         << " | Resource: " << r.name << endl;
}

void AllocationQueue::dequeue() {

    if(isEmpty()) {
        cout << "No patients in queue\n";
        return;
    }

    Node* temp = front;

    cout << "Released Patient: " << temp->patient.name << endl;

    front = front->next;
    if(front == nullptr) rear = nullptr;

    delete temp;
}

void AllocationQueue::display() {

    if(isEmpty()) {
        cout << "No active allocations.\n";
        return;
    }

    Node* t = front;
    cout << "\n--- Current Allocations ---\n";

    while(t){
        cout << "Patient: " << t->patient.name
        << " | Doctor: " << t->doctor.name
        << " | Resource: " << t->resource.name << endl;

        t = t->next;
    }
}

/*==================== SAVE / LOAD ====================*/

void AllocationQueue::saveToFile(const string& filename){

    ofstream f(filename);

    Node* t = front;

    while(t){

        f << t->patient.id << ","
          << t->patient.name << ","
          << t->doctor.id << ","
          << t->doctor.name << ","
          << t->resource.id << ","
          << t->resource.name << "\n";

        t=t->next;
    }

    f.close();
}

void AllocationQueue::loadFromFile(const string& filename){

    ifstream f(filename);
    if(!f) return;

    string line;
    while(getline(f,line)){

        stringstream ss(line);
        string temp;

        Patient p;
        Doctor d;
        Resource r;

        getline(ss,temp,','); p.id=stoi(temp);
        getline(ss,p.name,',');

        getline(ss,temp,','); d.id=stoi(temp);
        getline(ss,d.name,',');

        getline(ss,temp,','); r.id=stoi(temp);
        getline(ss,r.name);

        enqueue(p,d,r);
    }
}

/*==================== GLOBALS ====================*/

vector<Doctor> doctors;
vector<Resource> resources;

int nextDocID=1;
int nextResID=1;

AllocationQueue allocation;

/*==================== UTIL ====================*/

void clearInput(){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
}

/*==================== MENUS ====================*/

void runModule2Menu(){

    int c;

    while(true){

        cout << "\n==== MODULE 2 : DOCTOR & RESOURCE MANAGEMENT ====\n";
        cout << "1. Add Doctor\n";
        cout << "2. Add Resource\n";
        cout << "3. Allocate Patient\n";
        cout << "4. Release Patient\n";
        cout << "5. Show Allocations\n";
        cout << "6. Exit Module 2\n";
        cout << "Choice: ";

        cin >> c;

        if(c==1){

            Doctor d;
            d.id=nextDocID++;
            clearInput();
            cout<<"Name: "; getline(cin,d.name);
            cout<<"Specialization: "; getline(cin,d.specialization);
            d.available=true;
            doctors.push_back(d);
        }
        else if(c==2){

            Resource r;
            r.id=nextResID++;
            clearInput();
            cout<<"Resource Name: "; getline(cin,r.name);
            r.available=true;
            resources.push_back(r);
        }
        else if(c==3){

            Patient p;
            cout<<"Patient ID: "; cin>>p.id; clearInput();
            cout<<"Patient Name: "; getline(cin,p.name);

            Doctor d;
            Resource r;

            bool found=false;

            for(auto &x:doctors)
                if(x.available){
                    d=x;
                    x.available=false;
                    found=true;
                    break;
                }

            if(!found){ cout<<"No free doctor.\n"; continue;}

            found=false;

            for(auto &x:resources)
                if(x.available){
                    r=x;
                    x.available=false;
                    found=true;
                    break;
                }

            if(!found){ cout<<"No free resource.\n"; continue;}

            allocation.enqueue(p,d,r);
        }
        else if(c==4){

            if(allocation.isEmpty()){ cout<<"Queue empty\n"; continue;}

            Node* f = allocation.getFront();

            // free doctor + resource
            for(auto &x:doctors)
                if(x.id==f->doctor.id)
                    x.available=true;

            for(auto &x:resources)
                if(x.id==f->resource.id)
                    x.available=true;

            allocation.dequeue();
        }
        else if(c==5) allocation.display();
        else break;
    }
}
