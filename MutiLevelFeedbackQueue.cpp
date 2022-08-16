
#include <bits/stdc++.h>


using namespace std;
#define MAX_WAITTIME 40 // thoi gian cho doi toi da

struct History{
    char Name;
    int time_start;
    History(char name, int time)
        :Name(name),time_start(time)
    {}
};  

class Process{
    private:
        char name;
        int AT;  // Arrive time;
        int BT; // brust time;
        int Last_Ptime; // time cuoi duoc chiem dung cpu
        int RT; // time remain
   public:
        Process(const char &Name,const int &aT, const int &bT)
            : name(Name), AT(aT), BT(bT), Last_Ptime(aT), RT(bT)
        {}
        Process()
        {}
        char getName(){
            return name;
        }
        void setRT(){
            RT--;
        }
        int getRT(){
            return RT;
        }
        void setLast_PT(int x){
            Last_Ptime=x;
        }
        int getLast_PT(){
            return Last_Ptime;
        }
        int getAT(){
            return AT;
        }
};

class CPU{
    private:
        Process* P_Handle; // tien trinh dang duoc xu li boi CPU;
        int QUEUE_P; // hang doi CPU dang lm viec 1/2/3
        int Time; // Thoi gian  tien trinh chiem dung cpu
    public:
        CPU(){}
        void Processing(){
            P_Handle->setRT();
            Time++;
        }
        bool check_Finish(){
            if(P_Handle==NULL)
                return false;
            if(P_Handle->getRT()==0)
                return true;
            else return false;
        }
        bool check_Time(){  // kiem tra xem tien trinh con duoc quyen chiem dung CPU khong
            switch (QUEUE_P)
            {
            case 1: { if(Time==5) return false; 
                        else return true;}
            case 2: { if(Time==8) return false; 
                        else return true;}
            default: return true;
            }
        }
        Process* getP_Handle(){
            return P_Handle;
        }
        void setP_Handle(){
            P_Handle=NULL;
            Time=0;
            QUEUE_P=0;
        }
        void setP_Handle(Process* P , int Q){
            P_Handle=P;
            QUEUE_P=Q;
            Time=0;
        }
        int getQueue_P(){
            return QUEUE_P;
        }
};

int num_p;

queue <Process*> Q1; //  First queue using RR with tine=5;
queue <Process*> Q2; // Second queue using RR with tine=8;
queue <Process*> Q3; // Third queue using FCFS ;

list <Process*> list_P;// danh sach cac tien trinh;
list <Process*> Finish_P; // danh sach cac tien trinh da hoan thanh
list <History> CPU_His; // luu lich su CPU lam viec

void input(){
    cout<<" Nhap so luong tien trinh:";cin>>num_p;
    char c='A';
    for(int i=0;i<num_p;i++,c++){
        int AT,BT;
        cout<<"Nhap arrive time cua trinh "<<c<<": ";cin>>AT;
        cout<<"Nhap Brust time cua trinh "<<c<<": ";cin>>BT;
        Process* temp= new Process(c,AT,BT);
        list_P.push_back(temp);
    }
}

int main(){
    string out_put="";
    int iTime=0; // bo dem thoi gian
    freopen("data.txt","r",stdin); // chuyen du lieu tu file data.txt ra stdin
    input();
    list_P.sort([](Process* A,Process *B){  // sap sep theo thu tu thoi gian den
        return A->getAT()<B->getAT();
    });
    cout<<endl;
    for(auto i: list_P){
        cout<<i->getName()<<" "<<i->getAT() <<" " << i->getRT() <<endl;
    }
    CPU* cpu= new CPU();  cpu->setP_Handle();
    // thuc thi
    while (!(Finish_P.size()==num_p))
    {
        // Dua tien trinh vao hang doi
        while(list_P.size()!=0 &&list_P.front()->getAT()==iTime){
                Q1.push(list_P.front());
                out_put=out_put+"Arrive :"+list_P.front()->getName()+" to Q1\n";
                list_P.pop_front();
        }
        // chuyen muc cho tien trinh cho doi qua lau
        while (Q2.size()!=0 && (iTime - Q2.front()->getLast_PT())>=MAX_WAITTIME)
        {
            Q2.front()->setLast_PT(iTime);
            out_put=out_put+"up level :"+Q2.front()->getName()+"\n";
            Q1.push(Q2.front());
            Q2.pop();
        }
        while (Q3.size()!=0 && (iTime - Q3.front()->getLast_PT()) >=MAX_WAITTIME)
        {
            Q3.front()->setLast_PT(iTime);
            out_put=out_put+"up level :"+Q3.front()->getName()+"\n";
            Q2.push(Q3.front());
            Q3.pop();
        }
        // kiem tra tien trinh chiem dung Cpu 
        if(cpu->check_Finish()){
            out_put=out_put+"Finish :"+ cpu->getP_Handle()->getName()+"\n";
            Finish_P.push_back(cpu->getP_Handle());
            cpu->setP_Handle();
        }else if(!cpu->check_Time()){
            int temp=cpu->getQueue_P();
            switch (temp){
                case 1: {
                        out_put+=out_put+"Break "+cpu->getP_Handle()->getName()+" out CPU to Q2\n";
                        cpu->getP_Handle()->setLast_PT(iTime);
                        Q2.push(cpu->getP_Handle());cpu->setP_Handle();
                        break;
                        }
                case 2:{
                        out_put=out_put+"Break "+cpu->getP_Handle()->getName()+" out CPU to Q3\n";
                        cpu->getP_Handle()->setLast_PT(iTime);
                        Q3.push(cpu->getP_Handle());cpu->setP_Handle();
                        break;
                        }
            }
        }
             //// hoan tat qua trinh dua tien trinh vao cac hang doi
        // Xu li tien trinh

        if(cpu->getP_Handle()==NULL){  // neu cpu ranh
            if(Q1.size()!=0){
                out_put=out_put+ "CPU processing "+Q1.front()->getName()+"\n";
                cpu->setP_Handle(Q1.front(),1);
                Q1.pop();
            }else if(Q2.size()!=0){
                out_put=out_put+"CPU processing "+Q2.front()->getName()+"\n";
                cpu->setP_Handle(Q2.front(),2);
                Q2.pop();
            }else if(Q3.size()!=0){
                out_put=out_put+"CPU processing "+Q3.front()->getName()+"\n";   
                cpu->setP_Handle(Q3.front(),3);
                Q3.pop();
            }
            if(cpu->getP_Handle()!=NULL){
                cpu->Processing();
                CPU_His.push_back(History(cpu->getP_Handle()->getName(),iTime));
            }else{
                if(CPU_His.back().Name!='#'){
                    CPU_His.push_back(History('#',iTime));
                }
            }
            //
        }else{ // neu cpu bi chiem dung 
                int k=cpu->getQueue_P();  // Hang doi cua tien trinh dang chiem dung CPU
                if(!Q1.empty() && k!=1){  // neu hang doi Q1 co ton tai chuong trinh khac thi duoc quyen trung dung cpu
                    switch (k)
                    {
                        case 2:{  // neu la hang doi thi 2
                            out_put=out_put+" break "+cpu->getP_Handle()->getName()+" out CPU  back Q2 because "+Q1.front()->getName()+" is ready in Q1\n";
                            cpu->getP_Handle()->setLast_PT(iTime);
                            Q2.push(cpu->getP_Handle()); //  push back Q2;
                            out_put=out_put+ "CPU processing "+Q1.front()->getName()+"\n";
                            cpu->setP_Handle(Q1.front(),1);
                            Q1.pop();
                            break;
                        }
                        case 3:{
                            out_put=out_put+" break "+cpu->getP_Handle()->getName()+" out CPU back Q3  because "+Q1.front()->getName()+" is ready in Q1\n";
                            cpu->getP_Handle()->setLast_PT(iTime);
                            Q3.push(cpu->getP_Handle()); //  push back Q3;
                            out_put=out_put+ "CPU processing "+Q1.front()->getName()+"\n";
                            cpu->setP_Handle(Q1.front(),1);
                            Q1.pop();
                            break;
                        }
                    }
                }else if(!Q2.empty() && k==3){
                            out_put=out_put+" break "+cpu->getP_Handle()->getName()+" out CPU back Q3 because "+Q2.front()->getName()+" is ready in Q1\n";
                            cpu->getP_Handle()->setLast_PT(iTime);
                            Q3.push(cpu->getP_Handle()); //  push back Q3;
                            out_put=out_put+ "CPU processing "+Q2.front()->getName()+"\n";
                            cpu->setP_Handle(Q2.front(),2);
                            Q2.pop();
                }
                cpu->Processing(); // Xu li tien trinh chiem dung CPU
            }
        //-----------------------------------------------
        if(out_put!=""){
            cout<<"Time:"<<iTime<<endl;
            cout<<out_put;
            out_put.clear();
            out_put="";
        }
        iTime++;
    }
    CPU_His.pop_back();
    cout<<"Bieu do gant:"<<endl;
    printf("%10s:" ,"Time");
    for(auto i: CPU_His){
        printf("%5d", i.time_start);
    }
    printf("%5d",iTime-1);
    printf("\n%10s:" ,"Process");
    cout<<"  ";
    for(auto i: CPU_His){
        printf("%5c", i.Name);
    }
    cout<<endl;
    return 0;
}
