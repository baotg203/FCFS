#include <bits/stdc++.h>
using namespace std;

class Process{
private:
    int id; // process id
    int cpu; // cpu time burst
    int arrival; // time arrival in cpu
public:
    Process(){
        id = 0;
        cpu = 0;
        arrival = 0;
    }
    Process(int _id, int _cpu, int _arrival){
        id= _id;
        cpu = _cpu;
        arrival = _arrival;
    }

    void printGantt(Process p[], int n){
        Process process[n];
        vector<Process> CPU; // CPU queue
        vector<Process> orderCPU;
        char cpu[n][50];
        int currTime=0;
        int cnt=0;
        int wt[n];

        for(int i=0; i<n; i++){
            process[i]=p[i];
        }

        // set up the array
        for(int i=0; i<n; i++){
            wt[i]=0;
            for(int j=0; j<50; j++){
                cpu[i][j]='-';
            }
        }
       
        // browse each colume with current time = 0
        for(currTime=0; cnt<n || (!CPU.empty() || !orderCPU.empty()); currTime++){  
            // if arrive at current time
            for(int i=0; i<n; i++){
                if(p[i].arrival==currTime){
                    orderCPU.push_back(p[i]);
                    cnt++;
                }
            }
            // if in CPU has a process that reach at its end time
            if(!CPU.empty()){
                if(CPU[0].arrival+CPU[0].cpu==currTime)
                    CPU.erase(CPU.begin());
            }
            // output | for CPU doing nothing
            if(orderCPU.empty() && CPU.empty()){
                for(int i=0; i<n; i++){
                    cpu[i][currTime]='|';
                }
                continue;
            }
            // CPU doesn't have any process, push process from cpu queue to run
            if(!orderCPU.empty() && CPU.empty()){
                CPU.push_back(orderCPU[0]);
                orderCPU.erase(orderCPU.begin());
            }
            // 1 process take CPU and other processes in CPU queue
            if(!orderCPU.empty() && !CPU.empty()){
                for(int i=0; i<orderCPU.size(); i++){
                    wt[orderCPU[i].id-1]++;
                    orderCPU[i].arrival++;
                }
            }
            // output # for process running CPU
            if(!CPU.empty()){
                cpu[CPU[0].id-1][currTime]='#';
            }
        }
        cout<<'\n';
        for(int i=0; i<n; i++){
            cout<< "Process "<<i+1<<": ";
            for(int j=0; j<currTime; j++){
                cout<<cpu[i][j];
            }
            cout<<'\n';
        }
        cout<<'\n';
        double waitingTime=0;
        double turnAround=0;
        double CPUusage=0;
        for(int i=0; i<currTime-1; i++){
            if(cpu[0][i]!='|')
                CPUusage++;
        }
        for(int i=0; i<n; i++)
            waitingTime+=double(wt[i]);
        cout<<"CPU usage: "<<CPUusage*100/(currTime-1)<<"%\n";
        cout<<"Average waiting time: "<<waitingTime/n<<'\n';
        cout<<"Average response time: "<<waitingTime/n<<'\n';
        for(int i=0; i<n; i++){
            int tmp=0;
            bool flag=false;
            for(int j=0; j<currTime; j++){ 
                while(cpu[i][j]=='#'){
                    j++;
                    tmp=j;
                    flag=true;
                }
                if (flag)
                    break;
            }
            turnAround+=tmp-process[i].arrival;
        }
        cout<<"Average turn-around time: "<<double(turnAround)/n<<'\n';
    }
};

int main(){
    Process pArr[100], p;
    int n, cpu, arrival;
    cout << "Enter number of processes: ";
    cin >> n;

    for(int i=0; i<n; i++){
        cout << "\nProcess: " << i+1<<'\n';
        cout << "===> Burst time: ";
        cin >> cpu;
        cout << "===> Arrival time: ";
        cin >> arrival;
        Process process(i+1, cpu, arrival);
        pArr[i] = process;
    }

    p.printGantt(pArr, n);
}