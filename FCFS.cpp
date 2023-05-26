#include <bits/stdc++.h>
using namespace std;

class Process{
private:
    int id; // process id
    int cpu; // cpu time burst
    int arrival; // time arrival in cpu
    int io; // I/O time burst
    int ioStart; // time start I/O
    bool haveIO;
public:
    Process(){
        id = 0;
        cpu = 0;
        arrival = 0;
        io = -1;
        ioStart = -1;
        haveIO = false;
    }
    Process(int _id, int _cpu, int _arrival, int _io, int _ioTime, bool _haveIO){
        id= _id;
        cpu = _cpu;
        arrival = _arrival;
        haveIO = _haveIO;
        if(haveIO){
            io = _io;
            ioStart = _ioTime;
        }
        else{
            io = -1;
            ioStart = -1;
        }
    }

    void printGantt(Process p[], int n, bool IO){
        Process process[n];
        vector<Process> CPU; // CPU run
        vector<Process> ioRun; // IO run
        vector<Process> cpuQueue, ioQueue; // CPU queue | IO queue
        char cpu[n][50], io[n][50];
        int currTime=0;
        int cnt=0;
        int wt[n];
        int ta[n];
        int res[n];

        // set up the array
        for(int i=0; i<n; i++){
            wt[i]=0;
            res[i]=-1;
            for(int j=0; j<50; j++){
                cpu[i][j]='-';
                io[i][j]='-';
            }
        }

        for(int i=0; i<n; i++)
            process[i]=p[i];
       
        // browse each colume with current time = 0
        for(currTime=0; cnt<n || !CPU.empty() || !cpuQueue.empty() || !ioRun.empty() || !ioQueue.empty(); currTime++){  
            // if arrive at current time
            for(int i=0; i<n; i++){
                if(p[i].arrival==currTime){
                    cpuQueue.push_back(p[i]);
                    cnt++;
                }
            }

            if(!CPU.empty()){
                if(CPU[0].arrival+CPU[0].ioStart==currTime){
                    ioQueue.push_back(CPU[0]);
                    CPU.clear();
                }
            }
            // CPU doesn't have any process, push process from cpu queue to run
            while(!cpuQueue.empty() && CPU.empty()){
                CPU.push_back(cpuQueue[0]);
                if(res[CPU[0].id-1]==-1)
                    res[CPU[0].id-1]=currTime;
                if(CPU[0].arrival+CPU[0].ioStart==currTime){
                    ioQueue.push_back(CPU[0]);
                    CPU.clear();
                }
                cpuQueue.erase(cpuQueue.begin());
            }

            if(!ioRun.empty()){
                if(ioRun[0].arrival+ioRun[0].ioStart+ioRun[0].io==currTime){
                    ioRun[0].cpu+=ioRun[0].io;
                    cpuQueue.push_back(ioRun[0]);
                    ioRun.clear();
                }
            }
            
            if(ioRun.empty()){
                if(!ioQueue.empty()){
                    ioRun.push_back(ioQueue[0]);
                    ioQueue.erase(ioQueue.begin());
                }
            }
            if(!ioRun.empty()){
                if(!ioQueue.empty()){
                    for(int i=0; i<ioQueue.size(); i++){
                        ioQueue[i].io++;
                    }
                }
            }
            if(!CPU.empty()){
                if(CPU[0].arrival+CPU[0].cpu==currTime){
                    ta[CPU[0].id-1]=currTime;
                    CPU.clear();
                }
            }
            
            // CPU doesn't have any process, push process from cpu queue to run
            if(!cpuQueue.empty() && CPU.empty()){
                CPU.push_back(cpuQueue[0]);
                if(res[CPU[0].id-1]==-1)
                    res[CPU[0].id-1]=currTime;
                cpuQueue.erase(cpuQueue.begin());
            }

            
            while(!CPU.empty()){
                if(CPU[0].arrival+CPU[0].cpu==currTime){
                    ta[CPU[0].id-1]=currTime;
                    CPU.clear();
                }
                if(!cpuQueue.empty() && CPU.empty()){
                    CPU.push_back(cpuQueue[0]);
                    if(res[CPU[0].id-1]==-1)
                        res[CPU[0].id-1]=currTime;
                    cpuQueue.erase(cpuQueue.begin());
                }
                else break;
            }
            // 1 process take CPU and other processes in CPU queue
            if(!cpuQueue.empty() && !CPU.empty()){
                for(int i=0; i<cpuQueue.size(); i++){
                    cpuQueue[i].arrival++;
                }
            }
            if(CPU.empty()){
                if(cpuQueue.empty()){
                    for(int i=0; i<n; i++){
                        cpu[i][currTime]='|';
                    }
                }
            }
            if(!CPU.empty()){
                cpu[CPU[0].id-1][currTime]='#';
            }
            if(ioRun.empty()){
                if(ioQueue.empty())
                    for(int i=0; i<n; i++){
                        io[i][currTime]='|';
                    }
            }

            if(!ioRun.empty()){
                io[ioRun[0].id-1][currTime]='#';
            }

            if(!cpuQueue.empty()){
                for(int i=0; i<cpuQueue.size(); i++){
                    wt[cpuQueue[i].id-1]++;
                }
            }
        }
        cout<<"CPU: \n";
        for(int i=0; i<n; i++){
            cout<< "Process "<<i+1<<": ";
            for(int j=0; j<currTime; j++){
                cout<<cpu[i][j];
            }
            cout<<'\n';
        }
        cout<<"IO: \n";
        for(int i=0; i<n; i++){
            cout<< "Process "<<i+1<<": ";
            for(int j=0; j<currTime; j++){
                cout<<io[i][j];
            }
            cout<<'\n';
        }
        double waitingTime=0;
        double CPUusage=0;
        double response=0;
        double turnAround=0;
        for(int i=0; i<currTime-1; i++){
            if(cpu[0][i]!='|')
                CPUusage++;
        }
        for(int i=0; i<n; i++){
            response+=res[i]-process[i].arrival;
        }
        for(int i=0; i<n; i++){
            waitingTime+=double(wt[i]);
        }
        for(int i=0; i<n; i++){
            turnAround+=ta[i]-process[i].arrival;
        }
        cout<<"CPU usage: "<<CPUusage*100/(currTime-1)<<"%\n";
        cout<<"Average response time: "<<response/n<<'\n';
        cout<<"Average waiting time: "<<waitingTime/n<<'\n';
        cout<<"Average turn-around time: "<<double(turnAround)/n<<'\n';
    }
};

int main(){
    Process pArr[100], p;
    int n, x, cpu, arrival, io=-1, ioStart=-1;
    bool IO = false;
    cout << "Enter number of processes: ";
    cin >> n;
    cout << "\nDo the processes have IO? (note: 1 mean 'yes', 0 mean 'no') ";
    cin >> x;
    if(x == 1)
        IO=true;

    for(int i=0; i<n; i++){
        cout << "\nProcess: " << i+1<<'\n';
        cout << "===> Burst time: ";
        cin >> cpu;
        cout << "===> Arrival time: ";
        cin >> arrival;
        if(IO){
            cout <<"===> IO burst time: ";
            cin >> io;
            cout <<"===> IO start time: ";
            cin >> ioStart;
        }
        Process process(i+1, cpu, arrival, io, ioStart, IO);
        pArr[i] = process;
    }

    p.printGantt(pArr, n, IO);
}

