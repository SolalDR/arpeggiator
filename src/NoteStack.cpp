#include "NoteStack.h"
using namespace std;

#define DEBUG false

/*
 * Pile de notes
 * Toutes les notes jouées sont stocké dans "head"
 */
typedef struct NodeNote NodeNote;
struct NodeNote {
  int pitch;
  int endAt;
  struct NodeNote * next;
};

/*
 * Send a midi note
 * cmd: int => the channel 
 * pitch: int => [21 - 127] 
 * velocity: int
 */ 
void noteOn(int cmd, int pitch, int velocity) {
  if (DEBUG == false) {
    Serial.write(cmd);
    Serial.write(pitch);
    Serial.write(velocity); 
  }
}


// NoteStack::NoteStack() 
// {
//   head = (NodeNote *) malloc(sizeof(NodeNote));
// }

// void NoteStack::addNote(int pitch, int velocity, float endTime) {
//     // Trigger midi command
//   noteOn(0x90, pitch, velocity);

//   //NodeNote * current = getLast();
//   // Get the last item
//   NodeNote * current = head;

//   Serial.print("Next pointer : ");
//   NodeNote * c = head->next;
//   Serial.println((int) c);

//   int i = 0;
//   while (current->next != NULL) {    
//     current = current->next;
//     i = i+1;
//   }

//   Serial.print("count : ");
//   Serial.println(i);
  
//   // Allocate memory
//   current->next = (NodeNote *) malloc(sizeof(NodeNote)); 
  
//   // Register values
//   current->next->pitch = pitch;                          
//   current->next->endAt = endTime;
//   current->next->next = NULL;

//   Serial.print("Next pointer : ");
//   Serial.println((int) current->next);
//   Serial.println((int) current->next->next);

// }

// void NoteStack::removeOldNotes() {
//   NodeNote * current = this->head;
//   NodeNote * next;
//   Serial.println("Enter remove");

//   int currentTime = millis();

//   while (current != NULL) {
//     next = current->next;       
//     if(next != NULL && next->endAt < currentTime) {
//       Serial.println(next->pitch);
      
//       noteOn(0x90, next->pitch, 0x00);
//       // Si la note d'après existe
//       NodeNote * new_next = next->next;

//       free(next);
//       current->next = new_next;
//     }
//     current = next;
//   }
// }

// class NoteStack {
//   NodeNote * head = (NodeNote *) malloc(sizeof(NodeNote));

//   public: 

//   void addNote(int pitch, int velocity, float endTime) {
//      // Trigger midi command
//     noteOn(0x90, pitch, velocity);

//     //NodeNote * current = getLast();
//     // Get the last item
//     NodeNote * current = head;

//     Serial.print("Next pointer : ");
//     NodeNote * c = head->next;
//     Serial.println((int) c);

//     int i = 0;
//     while (current->next != NULL) {    
//       current = current->next;
//       i = i+1;
//     }

//     Serial.print("count : ");
//     Serial.println(i);
    
//     // Allocate memory
//     current->next = (NodeNote *) malloc(sizeof(NodeNote)); 
    
//     // Register values
//     current->next->pitch = pitch;                          
//     current->next->endAt = endTime;
//     current->next->next = NULL;

//     Serial.print("Next pointer : ");
//     Serial.println((int) current->next);
//     Serial.println((int) current->next->next);

//   }


//   void removeOldNotes() {
//     NodeNote * current = head;
//     NodeNote * next;
//     Serial.println("Enter remove");

//     int currentTime = millis();

//     while (current != NULL) {
//       next = current->next;       
//       if(next != NULL && next->endAt < currentTime) {
//         Serial.println(next->pitch);
        
//         noteOn(0x90, next->pitch, 0x00);
//         // Si la note d'après existe
//         NodeNote * new_next = next->next;

//         free(next);
//         current->next = new_next;
//       }
//       current = next;
//     }
//   }

//   int count() {
//     // Get the last item
//     NodeNote * current = head;
//     int i = 0;
//     while (current->next != NULL) {
//       current = current->next;
//       i++;
//     }
//     return i;
//   }
// };
