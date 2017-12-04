#include <iostream>
#include <fstream>
#include <ctime>
#include <stdlib.h>
#include <string.h>
#include <iomanip> 
#include <sstream> 
#include <string>
#include <math.h>
//#include <oled-exp.h>
//#include <onion-i2c.h>

using namespace std;

int displayWeather(string time, string date);	
int displayStats(string time, string date);
int display(string str);
string getWeather();
string getTemperature();
string getStatistics();

template <typename T>
string NumberToString ( T Number ){
	ostringstream ss;
	ss << Number;
	return ss.str();
}
 
int StringToInt(string line){
	int temp;
	stringstream ss(line);
	ss >> temp;
	return temp;
}

class GPIO{
	public: 
		void addPin(const char pin[]);
		void removePin(const char pin[]);
		void writeInOut(const char pin[], const int pinSize, const char direction[]);
		void writeValue(const char pin[], const int pinSize,const char value);
		void readValue(const char pin[], const int pinSize, char value[2]);
		GPIO(); //constructor 
		~GPIO(); //destructor 
};

void GPIO::addPin(const char pin[]){
			ofstream outfile;
			outfile.open("/sys/class/gpio/export");
			if (!outfile.is_open()){
				cout << "file did not open" << endl;
			} else {
				outfile << pin;
				//cout << "Pin " << pin << " added" << endl;
			}
			outfile.close();
		}

		void GPIO::removePin(const char pin[]){
			ofstream outfile;
			outfile.open("/sys/class/gpio/unexport");
			if (!outfile.is_open()){
				cout << "file did not open" << endl;
			} else {
				outfile << pin;
				//cout << "Pin " << pin << " removed" << endl;
			}
		}

		void GPIO::writeInOut(const char pin[], const int pinSize, const char direction[]){
			ofstream outfile;
			char filename[pinSize+31]="/sys/class/gpio/gpio";
			char end[11]="/direction";
			for (int i=20;i<pinSize+31;i++){
				if (i<pinSize+20){
					filename[i]=pin[i-20];
				} else if (i<pinSize+30){
					filename[i]=end[i-pinSize-20];
				} else {
					filename[i]=0;
				}
			}
			//cout << filename << endl;
			outfile.open(filename); //Open file
			if (!outfile.is_open()){
				cout << "file did not open" << endl;
			} else {
				outfile << direction;
				//cout << "Pin " << pin << " direction: " << direction << endl;
			}
			outfile.close(); //Close
		}

		void GPIO::writeValue(const char pin[], const int pinSize,const char value){
			ofstream outfile;
			char filename[pinSize+27]="/sys/class/gpio/gpio";
			char end[7]="/value";
			for (int i=20;i<pinSize+27;i++){
				if (i<pinSize+20){
					filename[i]=pin[i-20];
				} else if (i<pinSize+27){
					filename[i]=end[i-pinSize-20];
				} else {
					filename[i]=0;
				}
			}
			outfile.open(filename); //Open file
			if (!outfile.is_open()){
				cout << "file did not open" << endl;
			} else {
				outfile << value;
				//cout << "Pin " << pin << " status: " << value << endl;
			}
			outfile.close(); //Close
		}

void GPIO::readValue(const char pin[], const int pinSize, char value[2]){
	ifstream infile;
	char filename[pinSize+27]="/sys/class/gpio/gpio";
	char end[7]="/value";
	for (int i=20;i<pinSize+27;i++){
		if (i<pinSize+20){
			filename[i]=pin[i-20];
		} else if (i<pinSize+27){
			filename[i]=end[i-pinSize-20];
		} else {
		filename[i]=0;
	}
	}
			infile.open(filename);
			if (!infile.is_open()){
				cout << "file did not open" << endl;
			} else {
				char line[2];
			int flag=0;
			while(flag<1){ //Read File
			infile.getline(line,2);
			//cout << "line: " << line << endl;
			for (int i=0;i<2;i++){
				value[i]=line[i];
			}
			flag++;
		}
		//cout << "Pin " << pin << " status: " << value << endl;
	}
}

string getTime(int& hour, int& minute){
	//Get Time 
	time_t timeNow;
	struct tm * timeinfo;
	char buffer[80];
	time (&timeNow);
	timeinfo = localtime(&timeNow);
	strftime(buffer,sizeof(buffer),"%I:%M:%S",timeinfo);
	string str(buffer);
	char buffer1[80];
	time (&timeNow);
	timeinfo = localtime(&timeNow);
	strftime(buffer1,sizeof(buffer1),"%I",timeinfo);
	string hours(buffer1);
	hour=StringToInt(hours);
	char buffer2[80];
	time (&timeNow);
	timeinfo = localtime(&timeNow);
	strftime(buffer2,sizeof(buffer2),"%M",timeinfo);
	string minutes(buffer2);
	minute=StringToInt(minutes);
	return str;
}

string getDate(int& month, int& day){
	time_t timeNow;
	struct tm * timeinfo;
	char buffer[80];
	time (&timeNow);
	timeinfo = localtime(&timeNow);
	strftime(buffer,sizeof(buffer),"%Y/%m/%d",timeinfo);
	string str(buffer);
	char buffer1[80];
	time (&timeNow);
	timeinfo = localtime(&timeNow);
	strftime(buffer1,sizeof(buffer1),"%m",timeinfo);
	string months(buffer1);
	month=StringToInt(months);
	char buffer2[80];
	time (&timeNow);
	timeinfo = localtime(&timeNow);
	strftime(buffer2,sizeof(buffer2),"%d",timeinfo);
	string days(buffer2);
	day=StringToInt(days);
	return str;
}

class Alarm{
	private:
		int hour;
		int minute;
		string time;
	public:
		void addHour();
		void addMinute();
		void setAlarm();
		string getTime();
		int getHour();
		int getMinute();
		Alarm(); //constructor 
		~Alarm(); //destructor 
};

void Alarm::addHour(){
			int temp=hour;
			temp++;
			if (temp>24){
				temp=0;
			}
			hour=temp;
		}
		void Alarm::addMinute(){
			int temp=minute;
			temp++;
			if (temp>60){
				temp=0;
			}
			minute=temp;
		}
		void Alarm::setAlarm(){
			string hours;
			if (hour<10){
				hours="0"+NumberToString(hour);
			} else {
				hours=NumberToString(hour);
			}
			string minutes;
			if (minute<10){
				minutes="0"+NumberToString(minute);
			} else {
				minutes=NumberToString(minute);
			}
			time=hours+":"+minutes;
		}
		
		string Alarm::getTime(){
			return time;
		}
		int Alarm::getHour(){
			return hour;
		}
		int Alarm::getMinute(){
			
			return minute;
		}
		Alarm::Alarm(){
			hour=0;
			minute=0;
			time="00:00:00";
		}

float hoursSlept(int sleepStartHour, int sleepStartMinute, int sleepEndHour, int sleepEndMinute, 
int sleepStartMonth, int sleepStartDay, int sleepEndMonth, int sleepEndDay){
	//If the user sleeps and wakes up on the same date 
	float sleepMinutes=0;
	int daysPassed=sleepEndDay-sleepStartDay;
	int monthsPassed=sleepEndMonth-sleepStartMonth;
	int startMinutes=(sleepStartHour*60+sleepStartMinute);
	int endMinutes=(sleepEndHour*60+sleepEndMinute);
	if (daysPassed==0 && monthsPassed==0){
		sleepMinutes=endMinutes-startMinutes;
	} else if (daysPassed>0 && monthsPassed==0){
		sleepMinutes=24*60*daysPassed-startMinutes+endMinutes;
	} else if (monthsPassed>0){ 
		//Assuming they dont sleep for a month
		sleepMinutes=24*60-startMinutes;
		daysPassed=sleepEndDay-1;
		sleepMinutes+=24*60*daysPassed+endMinutes;
	}
	return sleepMinutes/60;
}

void writeStatFile(float hoursSlept, int hour, int minute, string date){
	ofstream outfile;
	string line;
	hour = hour-21;
	string hourLine;
	if (abs(hour)<10 && hour<0){
		hourLine="-0"+NumberToString(abs(hour));
	} else if (abs(hour)>10 && hour<0){
		hourLine="-0"+NumberToString(abs(hour));
	} else if (hour>=10){
		hourLine=NumberToString(hour);
	} else if (hour>=0 && hour <10){
		hourLine="0"+NumberToString(hour);
	}
	string minuteLine;
	if (minute<10){
		minuteLine="0"+NumberToString(minute);
	} else {
		minuteLine=NumberToString(minute);
	}
	stringstream stream;
	stream << fixed << setprecision(2) << hoursSlept;
	string temp = stream.str();
	line = temp + "," + hourLine + ":" + minuteLine + "," + date;
	cout << line << endl;
	outfile.open("SleepInfo.txt");
	if (!outfile.is_open()){
		cout << "file did not open" << endl;
	} else {
		outfile << line;
	}
	outfile.close();
}

//The optimal sleep time for healthy adults (18+) is 7.5-8.5 hours, says
//http://healthysleep.med.harvard.edu/need-sleep/what-can-you-do/assess-needs

// Struct recording data for each month
struct Month {
	int monthNum = 0;
	int numOfDays;
};

// Struct serving as array for month struct
struct Year {
	int numOfMonths = 0;
	Month* months;
};

// Struct recording data for each night slept
struct DailyStats {
	float hrsSlept;
	char timeSlept[5];
	char date[10];
	int combTime;
	int combDate;
	int minutes;
};

// Strut collecting statistics for one week of sleep
struct WeeklyStats {
	float minHrsSlept;
	float avgHrsSlept;
	float maxHrsSlept;
	float stdDev;
	string earliestTimeSlept;
	string latestTimeSlept;
	string avgTimeSlept;
};

// Node struct
struct Node {
	DailyStats dStats;
	Node* next;
};

// Method declaration
string rewriteTime(int time);
bool addNode(Node*& list, const DailyStats& dayRecords);
bool deleteNode(Node*& list, const int day);
void displayList(const Node* list);
DailyStats* get(Node* list, int index);
int testMethod(const char filename[], DailyStats ds, Node*& records, int& daysRecorded);
int computeStatistics(Node*& records, WeeklyStats& ws, const int daysRecorded);
int writeFile(const char filename[], const WeeklyStats& ws);

// Method for adding node to list
bool addNode(Node*& list, const DailyStats& dayRecords) {
  Node* newNode = new Node;
  if (newNode) {
    newNode->dStats = dayRecords;
    newNode->next = list;
    list = newNode;
    return true;
  }
  return false;
}

// Method for removing node from list
bool deleteNode(Node*& list, const int day) {
	Node* cookie = list;

	if (cookie == 0)
		return false;
	if (cookie->dStats.combDate == day) {
		list = list->next;
		delete cookie;
		return true;
	}
	while (cookie->next) {
    if (cookie->next->dStats.combDate == day) {
      Node* tmp = cookie->next;
      cookie->next = cookie->next->next;
      delete tmp;
      return true;
    }
    cookie = cookie->next;
  }
  return false;
}

void displayList(const Node* list) {
	const Node* cookie = list;
	int i = 0;
	while (cookie) {
		cout << i << ": " << cookie->dStats.combTime << " --> ";
		cookie = cookie->next;
		i++;
	}
	cout << "EOL" << endl;
}

DailyStats* get(Node* list, int index) {
	Node* cookie = list;
	int counter = 0;

	if (index == 0) {
		return &(cookie->dStats);
	}

	while (cookie->next) {
		cookie = cookie->next;
		counter++;
		if (counter == index) {
			return &(cookie->dStats);
		}
	}
	return NULL;
}

int testMethod(DailyStats ds, Node*& records, int& daysRecorded) {
	const int maxLineLength = 100;
	char line[maxLineLength];
	ifstream infile;
	infile.open("SleepInfo.txt");

	if (!infile.is_open()) {
		return -1;
	}

	bool done = false;
	int fileLineNumber = 0;

	if(!infile.getline(line, maxLineLength)) {
		if (infile.eof()) {
			done = true;
		}
		else
			return -1;
		}

    while (!done) {
		float tempHrsSlept = 0;
		float tempHSAfterDec = 0;

		int tempBedTime = 0;	
		int tempHour = 0;
		int tempMinute = 0;
		int minutesFromRef = 0;
		int cMinutesFromRef = 0;

		int tempDate = 0;
		int tempYear = 0;
		int tempMonth = 0;
		int tempDay = 0;

		int sign = 1;

		bool comma1 = false;
		bool comma2 = false;
		bool decCheck = false;
		bool colonCheck = false;
		bool slash1 = false;
		bool slash2 = false;

		for (int i = 0; line[i] != 0; i++) {
			if (line[i] != ',' && !comma1 && !comma2) { // Detect hours slept
				if (line[i] == '.') {
					decCheck = true;
				}
				else if (line[i] >= '0' && line[i] <= '9' && !decCheck) {
					tempHrsSlept = tempHrsSlept*10 + line[i] - '0';
				} 
				else if (line[i] >= '0' && line[i] <= '9' && decCheck) {
					tempHSAfterDec = tempHSAfterDec*10 + line[i] - '0';
				}
			tempHrsSlept = tempHrsSlept + (tempHSAfterDec/10);
			} 
			else if (line[i] == ',' && !comma1 && !comma2) { // Detect comma after hours slept
				comma1 = true;
			} 
			else if (line[i] != ',' && comma1 && !comma2) { // Detect bedtime -- Note that the bedtime is relative to 9 PM. (Ex. 0311 = 12:11 AM)
				if (line[i] >= '0' && line[i] <= '9' && !colonCheck) {
					tempHour = tempHour*10 + line[i] - '0';
					tempBedTime = tempBedTime*10 + line[i] - '0';
					minutesFromRef = minutesFromRef*10 + line[i] - '0';
				}
				else if (line[i] >= '0' && line[i] <= '9' && colonCheck) {
					tempMinute = tempMinute*10 + line[i] - '0';
					tempBedTime = tempBedTime*10 + line[i] - '0';
					cMinutesFromRef = cMinutesFromRef*10 + line[i] - '0';
				} 
				else if (line[i] == '-') {
					sign = -1;
				} 
				else if (line[i] == ':') {
					colonCheck = true;
				}
			} 
			else if (line[i] == ',' && comma1 && !comma2) { // Detect comma after bedtime
				comma2 = true;
			} 
			else if (line[i] != ',' && comma1 && comma2) { // Detect date
				if (line[i] >= '0' && line[i] <= '9' && !slash1 && !slash2) {
					tempYear = tempYear*10 + line[i] - '0';
					tempDate = tempDate*10 + line[i] - '0';
				} 
				else if (line[i] >= '0' && line[i] <= '9' && slash1 && !slash2) {
					tempMonth = tempMonth*10 + line[i] - '0';
					tempDate = tempDate*10 + line[i] - '0';
				} 
				else if (line[i] >= '0' && line[i] <= '9' && slash1 && slash2) {
					tempDay = tempDay*10 + line[i] - '0';
					tempDate = tempDate*10 + line[i] - '0';
				} 
				else if (line[i] == '/' && !slash1 && !slash2) {
					slash1 = true;
				} 
				else if (line[i] == '/' && slash1 && !slash2) {
					slash2 = true;
				}
			}
		}
    char tBedTime[5];
    char tDate[10];

    //char array for bedtime
    if (tempHour < 10) {
		tBedTime[0] = '0';
    }
    else {
		tBedTime[0] = tempHour / 10 + '0';
    }
	tBedTime[1] = tempHour % 10;
	tBedTime[2] = ':';

    if (tempMinute < 10) {
        tBedTime[3] = '0';
      }
      else {
        tBedTime[3] = tempMinute / 10 + '0';
      }
      tBedTime[4] = tempMinute % 10;
      //char array for date
      tDate[0] = '2';
      for (int i = 1; i < 4; i++) {
        tDate[i] = (int)(tempYear/pow(10, 3-i)) % (int)(tempYear/pow(10, 4-i)) + '0';
      }

      tDate[5] = '/';

      if (tempMonth < 10) {
        tDate[6] = '0';
        tDate[7] = tempMonth + '0';
      }

      else {
        tDate[6] = '1';
        tDate[7] = tempMonth % 10 + '0';
      }
      tDate[8] = '/';

      if (tempDay < 10) {
        tDate[9] = '0';
        tDate[10] = tempDay + '0';
      }

      else {
        tDate[9] = tempDay / 10 + '0';
        tDate[10] = tempDay % 10 + '0';
      }
      //

      for (int i = 0; i < 5; i++) {
        ds.timeSlept[i] = tBedTime[i];
      }

      for (int i = 0; i < 10; i++) {
        ds.date[i] = tDate[i];
      }
      minutesFromRef = (minutesFromRef*60 + cMinutesFromRef)*sign;
      tempBedTime = tempBedTime*sign;

      ds.combTime = tempBedTime;
      ds.hrsSlept = tempHrsSlept;
      ds.combDate = tempDate;
      ds.minutes = minutesFromRef;

      addNode(records, ds);
      daysRecorded++;

      ++fileLineNumber;
  		if (!infile.getline(line, maxLineLength)) {
  			if (infile.eof()) {
  				done = true;
  			}
  			else {
  				return -1;
  			}
  		}
    }
}

int computeStatistics(Node*& records, WeeklyStats& ws, const int daysRecorded) {
	if (daysRecorded  == 0) {
		return -1;
	}

	float tempMin = get(records, 0)->hrsSlept;
	float tempMax = get(records, 0)->hrsSlept;
	float sumHrsSlept = get(records, 0)->hrsSlept;

	for (int i = 1; i < daysRecorded; i++) {
		if (tempMin > get(records, i)->hrsSlept) {
			tempMin = get(records,i)->hrsSlept;
		}

		if (tempMax < get(records, i)->hrsSlept) {
			tempMax = get(records, i)->hrsSlept;
		}
		sumHrsSlept += get(records, i)->hrsSlept;
	}

	ws.minHrsSlept = tempMin;
	ws.maxHrsSlept = tempMax;
	ws.avgHrsSlept = sumHrsSlept/daysRecorded;

	int earliest = get(records, 0)->combTime;
	int latest = get(records, 0)->combTime;
	int sumTimeSlept = get(records, 0)->combTime;
	int minSum = get(records, 0)->minutes;
	float msAverage = 0;
	float dSum = 0;
	int averageTS = 0;

	for (int i = 1; i < daysRecorded; i++) {
		if (earliest > get(records, i)->combTime) {
			earliest = get(records, i)->combTime;
		}

    if (latest < get(records, i)->combTime) {
      latest = get(records, i)->combTime;
    }
    sumTimeSlept += get(records, i)->combTime;
    minSum += get(records, i)->minutes;
  }
  averageTS = sumTimeSlept/daysRecorded;
  msAverage = minSum/daysRecorded;

  for (int i = 0; i < daysRecorded; i++) {
    dSum += pow((get(records, i)-> minutes - msAverage),2);
  }

  ws.stdDev = sqrt(dSum/daysRecorded);

  ws.earliestTimeSlept = rewriteTime(earliest);
  ws.latestTimeSlept = rewriteTime(latest);
  ws.avgTimeSlept = rewriteTime(averageTS);

  return 0;
}

int addToList(Node*& records, DailyStats ds, int currentDate, int currentTime) { // run method when alarm is turned off
  int snoozeCounter;
}

int writeFile(const WeeklyStats& ws) {
  ofstream outfile;
	outfile.open("SleepStatistics.txt");
	if (!outfile.is_open()) {
		return -1;
	}

	outfile << "Min: " << ws.minHrsSlept << "hrs \r\n" << endl;
	outfile << "Max: " << ws.maxHrsSlept << "hrs \r\n" << endl;
	outfile << "Avg: " << ws.avgHrsSlept << "hrs \r\n" << endl;
	//outfile << "Earliest BT: " << ws.earliestTimeSlept << "\n" << endl;
	//outfile << "Latest BT: " << ws.latestTimeSlept << "\n" << endl;
	//outfile << "Avg BT: " << ws.avgTimeSlept << "\r\n" << endl;
	//outfile << "Deviation: " << ws.stdDev << "\r\n" << endl;

  if (ws.avgHrsSlept < 7.5) {
    outfile << "Not Well :(" << endl;
  }

  else if (ws.avgHrsSlept > 7.5 && ws.avgHrsSlept < 8.5) {
    outfile << "Well :)" << endl;
  }

  else if (ws.avgHrsSlept > 8.5) {
    outfile << "Too Much!" << endl;
  }

	outfile.close();
	return 0;
}

string rewriteTime(int time) {
	bool pos = true;
	if (time < 0) {
		pos = false;
	}

	char newTime[5];
	time += 2100;

	if (time >= 2400) {
		time -= 2400;
	}

	if (!pos) {
		if (time % 100 != 0) {
			time = (time / 100) * 100 + (time % 100) - 40;
		}
		else {
			time = (time / 100) * 100;
		}
	}

	int refMins = 60;
	int misc = 60 - (time % 100);

	for (int i = 0; i < 5; i++) {
		if (time < 1000) {
			newTime[0] = '0';

			if (time < 100) {
				newTime[1] = '0';

				if (time < 10) {
					newTime[3] = '0';

					if (time < 1) {
						newTime[4] = '0';
					}
					else {
						newTime[4] = time  + '0';
					}
				}
				else { // else < 10
					newTime[3] = (time / 10) + '0';
					newTime[4] = (time % 10) + '0';
				}
			}

      else { // else < 100
        newTime[1] = (time/100) + '0';
        newTime[3] = ((time / 10) % 10) + '0';
        newTime[4] = (time % 10) + '0';
      }
    }

    else { // else < 1000
      newTime[0] = time/1000 + '0';
      newTime[1] = ((time/100) % 10) + '0';
      newTime[3] = ((time/10) % 10) + '0';
      newTime[4] = (time % 10) + '0';
      }
    }
    newTime[2] = ':';
  return newTime;
}

string displayWeather() {
	if (getTemperature() == "" || getWeather() == "") {
		//display("Error reading from file");
		return NULL;
	}
	string str = "Temp: " + getTemperature() + " degrees    Weather: " + getWeather();
	return str;
}

string getWeather() {
  // get line from HTML file
  ifstream infile;
	infile.open("weather.txt");
	if (!infile.is_open()) {
		return "";
	}
  const int length = 1000;
	char line[length];
  if (!infile.getline(line, length)) {
    return "";
  }
  infile.close();

  // find weather condition in text
  string weather;
  int j = 0;
  for (int i = 0; i < length; i++) {
    if (line[i - 5] == 'm' && line[i - 4] == 'a' && line[i - 3] == 'i' && line[i - 2] == 'n' && line[i + 1] == '"') {
      bool done = false;
      while (!done) {
        weather += line[i + j + 2];
        j++;
        if (line[i + j + 2] == '"') {
          done = true;
        }
      }
    }
  }

  return weather;
}

string getTemperature() {
	// get line from HTML file
	ifstream infile;
	infile.open("weather.txt");
	if (!infile.is_open()) {
		return "";
	}
	const int length = 1000;
	char line[length];
	if (!infile.getline(line, length)) {
		return "";
	}
	infile.close();

	// find temperature in text
	string temperature;
	int j = 0;
	for (int i = 0; i < length; i++) {
		if (line[i - 5] == 't' && line[i - 4] == 'e' && line[i - 3] == 'm' && line[i - 2] == 'p' && line[i] == ':') {
			bool done = false;
				while (!done) {
					temperature += line[i + j + 1];
					j++;
					if (line[i + j + 1] == ',') {
					done = true;
				}
			}
		}
	}
  return temperature;
}

int getStatistics(string& str0,string& str1,string& str2,string& str3,string& str4,string& str5) {
	// get line from HTML file
	ifstream infile;
	infile.open("SleepStatistics.txt");
	if (!infile.is_open()) {
		return -1;
	}
	const int length = 1000;
	char line[length];
	bool done = false;
	int i=0;
	while (!done) {
		if (!infile.getline(line, length)) {
			if (infile.eof()) {
				done = true;
			} else {
				return -1;
			}
		}
		if (i==0){
			str0+= line;
		} else if (i==1){
			str1+=line;
		} else if (i==2){
			str2+=line;
		} else if (i==3){
			str3+=line;
		} else if (i==4){
			str4+=line;
		} else if (i==5){
			str5+=line;
		}
		i++;
	}
	infile.close();
	return 0;
}

int main(int args,char* argv[]){
	//declare my GPIO
	GPIO myPin; 
	//Reset pins
	myPin.removePin("0");
	myPin.removePin("1");
	myPin.removePin("2");
	myPin.removePin("3");
	myPin.removePin("6");
	//Add output pins (6)
	myPin.addPin("6");
	myPin.writeInOut("6",1,"out");
	//Add snooze button input pin(1)
	myPin.addPin("1");
	myPin.writeInOut("1",1,"in");
	//Add weight sensing input pin(3)
	myPin.addPin("3");
	myPin.writeInOut("3",1,"in");
	char value0[2]; //tracks value of pin0
	char value1[2]; //tracks value of pin1
	char value2[2]; //tracks value of pin2 
	char value3[2]; //tracks value of pin3
	//Decalare Alarm Time
	Alarm myAlarm;
	bool alarmReset=false;
	//Timer loop start 
	const int NUM_SECONDS=1;
	int count=1;
	int valueCount1=0;
	double time_counter=0;
	clock_t startTime = clock();
	clock_t last_time = startTime;
	//ONLY FOR DEMO PURPOSES
	int secondsToDelay=atoi(argv[1]);
	//Boolean variables
	bool flag=true;
	bool held=false;
	bool onBed=false;
	bool alarmOn=false;
	//Sleep variables 
	string sleepStartTime;
	string sleepEndTime;
	string sleepStartDate;
	string sleepEndDate;
	int sleepStartHour;
	int sleepStartMinute;
	int sleepEndHour;
	int sleepEndMinute;
	int sleepStartDay;
	int sleepEndDay;
	int sleepStartMonth;
	int sleepEndMonth;
	int alarmCounter=31;
	int currentHour;
	int currentMinute;
	int currentDay;
	int currentMonth;
	char init [100]={};
	string previousTime=getTime(currentHour,currentMinute).substr(0,5);
	strcpy(init,(getTime(currentHour,currentMinute).substr(0,5) + "   " + getDate(currentMonth,currentDay)).c_str());
	char* initPointer = init;
	int displayFlag=false;
	int displayCounter=0;
	int onBedCounter=0;
	//Call screen update
	oledDriverInit(); //initiate the oled expansion screen 
	oledClear();
	oledWrite(initPointer);
	Node* records = 0;
	//Start timer loop
	while(flag){
		startTime=clock();
		time_counter+=(double)(startTime-last_time);
		last_time=startTime;
		if (time_counter>(double)(NUM_SECONDS*CLOCKS_PER_SEC)){
			time_counter-=(double)(NUM_SECONDS*CLOCKS_PER_SEC);
			count++;
			myPin.readValue("1",1,value1); //Read the value of pin1 (input)
			myPin.readValue("3",1,value3); //Read the value of pin3 (input)
			cout << getTime(currentHour,currentMinute) << endl;
			cout << getDate(currentMonth,currentDay) << endl;
			//Update the time if the current time on OLED display if there is a change in minutes and the alarm is not on and the user is not currently
			//reseting the alarm and the display has been shown for longer than 10 seconds 
			if (getTime(currentHour,currentMinute).substr(0,5) != previousTime && alarmOn==false && alarmReset==false && displayCounter>10){	
				char temp [100]={};
				strcpy(temp,(getTime(currentHour,currentMinute).substr(0,5) + "   " + getDate(currentMonth,currentDay)).c_str());
				char* pointer = temp;
				//Call screen update
				oledScrollStop ();
				oledClear();
				oledWrite(pointer);
			}
			previousTime = getTime(currentHour,currentMinute).substr(0,5); //set previous time 
			if (onBed){ //The alarm can only be set when the user is on the bed 
				//Alarm goes off the current time is equal to the set alarm time
				if (myAlarm.getHour()==currentHour && myAlarm.getMinute()==currentMinute && alarmCounter>10){
					cout << "ring ring!" << endl;
					oledClear();
					oledScrollStop ();
					char alarmText[11] = "ring ring!";
					char* alarmPointer=alarmText;
					oledWrite(alarmPointer);
					alarmOn=true; //value signalling that the alarm is on
				}
				if (alarmOn==true && atoi(value1)==1){ //If the snooze button is pressed when the alarm is going off, allow snooze of 10s
					alarmCounter=0;
					oledClear();
					char snoozeText[26] = "Zzzzzzzzzzzzzzzzzzzzz";
					char* snoozePointer=snoozeText;
					oledWrite(snoozePointer);
					oledScroll(0, OLED_EXP_SCROLL_SPEED_5_FRAMES, 0, OLED_EXP_CHAR_ROWS - 1);
				}
			}
			if (alarmOn){ //if the alarm is currently on increase the counter, the only way to turn off the alarm is to get off the bed 
				alarmCounter++;
			}
			if (displayFlag){ //The display musta show for at least 10 seconds 
				displayCounter++;
			}
			if (onBed){ //If the user is onBed for more than 15min (demo purposes: 10 seconds), then they are considered to be asleep
				onBedCounter++;
			}
			if (atoi(value3)==1 && onBed==false){ //When the user gets on the bed
				onBed=true;
				cout << "user is on bed " << endl;
				sleepStartTime= getTime(sleepStartHour,sleepStartMinute);
				sleepStartDate= getDate(sleepStartMonth,sleepStartDay);
				onBedCounter++;
				if (onBedCounter>10){
					cout << "sleeping" << endl;
				}
			} else if (atoi(value3)==0 && onBed==true){
				cout << "user got off the bed" << endl;
				onBed=false;
				if (onBedCounter>10){ //User has woken up from sleep 
					cout << "sleeping" << endl;
					sleepEndTime = getTime(sleepEndHour,sleepEndMinute);
					sleepEndDate = getDate(sleepEndMonth,sleepEndDay);
					cout << hoursSlept(sleepStartHour, sleepStartMinute, sleepEndHour, sleepEndMinute, 
					sleepStartMonth,sleepStartDay, sleepEndMonth, sleepEndDay) << endl;
					//Write stats file
					writeStatFile(hoursSlept(sleepStartHour, sleepStartMinute, sleepEndHour, sleepEndMinute, 
					sleepStartMonth,sleepStartDay, sleepEndMonth, sleepEndDay), sleepStartHour, sleepStartMinute, sleepStartDate);
					DailyStats ds = {0};
					WeeklyStats ws = {0};
					int daysRecorded = 0;
					int cS = 0;
					testMethod(ds, records, daysRecorded);
					displayList(records);
					cout << "Dispaying records" << endl;
					cS = computeStatistics(records, ws, daysRecorded);
					if (cS < 0) {
						cout << "Error, not enough statistics to perform calculations" << endl;
					}
					writeFile(ws);
					char timeChar [100]={};
					strcpy(timeChar,(getTime(currentHour,currentMinute).substr(0,5) + "   " + getDate(currentMonth,currentDay)).c_str());
					char* timePointer = timeChar;
					char weatherChar[100]={};
					string weather = displayWeather();
					strcpy(weatherChar,weather.c_str());
					char* weatherPointer=weatherChar;
					char statsChar0[100]={};
					char statsChar1[100]={};
					char statsChar2[100]={};
					char statsChar3[100]={};
					char statsChar4[100]={};
					char statsChar5[100]={};
					string stat0;
					string stat1;
					string stat2;
					string stat3;
					string stat4;
					string stat5;
					getStatistics(stat0,stat1,stat2,stat3,stat4,stat5);
					cout << stat0 << endl;
					strcpy(statsChar0,stat0.c_str());
					strcpy(statsChar1,stat1.c_str());
					strcpy(statsChar2,stat2.c_str());
					strcpy(statsChar3,stat3.c_str());
					strcpy(statsChar4,stat4.c_str());
					strcpy(statsChar5,stat5.c_str());
					char* statsPointer0=statsChar0;
					char* statsPointer1=statsChar1;
					char* statsPointer2=statsChar2;
					char* statsPointer3=statsChar3;
					char* statsPointer4=statsChar4;
					char* statsPointer5=statsChar5;
					//Call screen update
					oledClear();
					oledScrollStop ();
					cout << statsPointer0 << endl;
					oledSetTextColumns(0);
					oledSetCursor(0, 0);
					oledWrite(timePointer);
					oledSetCursor(1, 0);
					oledWrite(weatherPointer);
					oledSetCursor(3, 0);
					oledWrite(statsPointer0);
					oledSetCursor(4, 0);
					oledWrite(statsPointer1);
					oledSetCursor(5, 0);
					oledWrite(statsPointer2);
					oledSetCursor(6, 0);
					oledWrite(statsPointer3);
					oledSetCursor(7, 0);
					oledWrite(statsPointer4);
					displayFlag=true;
					displayCounter=0;
					if (alarmOn){
						alarmOn=false;
						alarmCounter=11;
					}
				}
				onBedCounter=0;
			} 
			//Set the alarm
			if (alarmReset){
				//Set pin 0 and 2 to recieve input (hour and minute buttons)
				myPin.addPin("0"); //Hour input
				myPin.addPin("2"); //Minute input
				myPin.writeInOut("0",1,"in");
				myPin.writeInOut("2",1,"in");
				myPin.readValue("0",1,value0);
				myPin.readValue("2",1,value2);
				if (atoi(value0)==1){ //hour pressed
					myAlarm.addHour();
					cout << myAlarm.getHour() << endl;
				}
				if (atoi(value2)==1){
					myAlarm.addMinute();
					cout << myAlarm.getMinute() << endl;
				}
				string hours;
				if (myAlarm.getHour()<10){
					hours="0"+NumberToString(myAlarm.getHour());
				} else {
					hours=NumberToString(myAlarm.getHour());
				}
				string minutes;
				if (myAlarm.getMinute()<10){
					minutes="0"+NumberToString(myAlarm.getMinute());
				} else {
					minutes=NumberToString(myAlarm.getMinute());
				}
				string currentAlarm=hours+":"+minutes;
				char temp1[100]={};
				strcpy(temp1,currentAlarm.c_str());
				cout << temp1 << endl;
				char* currentAlarmPointer=temp1;
				oledClear();
				oledScrollStop ();
				oledWrite(currentAlarmPointer);
			}
			//To reset the alarm, the user has to hold the snooze button for 3 seconds 
			if (atoi(value1)==1){
				if (alarmReset && held==false){
					cout << "alarm time reset!" << endl;
					alarmReset=false;
					myAlarm.setAlarm();
					string resetMsg="New Alarm Time: " + myAlarm.getTime();
					char temp2 [100]={};
					strcpy(temp2,resetMsg.c_str());
					char* resetPointer=temp2;
					oledScrollStop ();
					oledClear();
					oledWrite(resetPointer);
					cout << "new alarm time: " << myAlarm.getTime() << endl;
					//Remove pins
					myPin.removePin("2");
					myPin.removePin("0");
				} else {
					valueCount1++;
					held=true;
					if (valueCount1==3){ //If the snooze button has been pressed for 3 seconds
						string msg="Resetting the alarm...";
						char temp3[100]={};
						strcpy(temp3,msg.c_str());
						char* msgPointer=temp3;
						oledClear();
						oledScrollStop ();
						oledWrite(msgPointer);
						alarmReset=true;
					}
				}
			} else {
				held=false;
				valueCount1=0;
			}
			/*if (count>=secondsToDelay){
				cout << secondsToDelay << " seconds have passed" << endl;
				flag=false;
			}*/	
		}
	}
	return 0;
}