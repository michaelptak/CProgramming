#include <stdio.h>
#include <string.h>

struct dataRecord {
  int intID;
  int intAge;
  char strName[100];
  float salary;
};

void printDataRecord(const struct dataRecord *p){
  printf("%s is %d years old, their salary is $%f\n", p->strName, p->intAge, p->salary);
}

int main() {
  struct dataRecord John; 
  John.intID=12345;
  John.intAge=25;
  John.salary=15.69;
  strcpy(John.strName, "John");
  printDataRecord(&John); // Print data record at address of John

  John.intAge=26;
  printDataRecord(&John);
  
  struct dataRecord *alias;
  alias=&John;
  alias->salary += 0.75;
  printDataRecord(&John);

  return 0;
}
