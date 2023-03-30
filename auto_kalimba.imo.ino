
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <Servo.h>

// int x_pos = 20;
// int y_pos = 0;
double tab_length = 0.7;
double tab_height = 0.4;
double x_r = 4.5;
double y_r = 1.6;
double teeth = 16;

class Note{
    public:
        String note;
        int note_num;
};
/*
int main() {
  String notes = "F1E1C1";
  int splitLength = 2;
  int NumSubstrings = notes.length() / splitLength;
  double moves[NumSubstrings * 6 + 3];

  int num = 3;
  int * count = &num;
  Parse_Notes(notes, moves, count);


  double final_moves[*count];

  for (int i = 0; i < *count; i ++){
    final_moves[i] = moves[i];
    }
    

  for (int i = 0; i < *count; i++) {
    std::cout << "Move #  ";
    std::cout << i;
    std::cout << "\n";
    std::cout << final_moves[i];
    std::cout << "\n";
  }

  
}
*/

void Parse_Notes(String notes, double *moves, int* count) {
  int splitLength = 2;
  int NumSubstrings = notes.length() / splitLength;

  String ret[NumSubstrings];

  for (auto i = 0; i < NumSubstrings; i++) {
    ret[i] = (notes.substring(i * splitLength, splitLength));
  }

  // If there are leftover characters, create a shorter item at the end.

  Note initial_note;
  initial_note.note = "C1";
  initial_note.note_num = 0;
  int initial_pos[2] = {0, 0};

  double x_pos = 20.0;
  double y_pos = 0.0;

  moves[0] = x_pos;
  moves[1] = y_pos;
  moves[2] = 0;
  int i = 3;
  int string_count = 0;
  while (i  < NumSubstrings * 6 + 3 && string_count < NumSubstrings) {

    Note note = Transform_Note(ret[string_count]);
    string_count ++;

    int pos[2];
    Note_To_Direction(note, pos);

    // Checked up to here

    double move_ptr[6];
    Check_Move(pos, initial_pos, x_pos, y_pos, move_ptr);


    moves[i] = move_ptr[0];
    i++;
    moves[i] = move_ptr[1];
    i++;
    moves[i] = move_ptr[2];
    i++;
    *(count) += 3;
    
    double x_y = *(move_ptr + 2);

    if (x_y == 0) {
      x_pos = *move_ptr;
      y_pos = *(move_ptr + 1);
    } else {
      y_pos = *move_ptr;
      x_pos = *(move_ptr + 1);
    }
    
    if (move_ptr[3] != -1) {
      moves[i] = move_ptr[3];
      i ++;
      moves[i] = move_ptr[4];
      i ++;
      moves[i] = move_ptr[5];
      i ++;

      x_y = move_ptr[5];
      if (x_y == 0) {
      x_pos = move_ptr[3];
      y_pos = move_ptr[4];
    } else {
      y_pos = move_ptr[3];
      x_pos = move_ptr [4];
    }

      
      *count += 3;
    }

    initial_note = note;
    for (int j = 0; j < 2; j++) {
      initial_pos[j] = pos[j];
    }
  }
}

void Check_Move(int *pos, int *initial_pos, double x_pos, double y_pos,
                double *move_ptr) {
  // Takes in x and y displacement and turn that into motor commands

  int arr[] = {0, 0};
  int *zero_pos = arr;

  double arr2[6];

  double temp_ptr[3];

  if (initial_pos[1] > 0 && pos[1] < 0 || initial_pos[1] < 0 && pos[1] > 0) {

    Move(zero_pos, initial_pos, x_pos, y_pos, temp_ptr);
    
    move_ptr[0] = temp_ptr[0];
    move_ptr[1] = temp_ptr[1];
    move_ptr[2] = temp_ptr[2];
    

    Move(pos, zero_pos, 90, 0, temp_ptr);

    move_ptr[3] = temp_ptr[0];
    move_ptr[4] = temp_ptr[1];
    move_ptr[5] = temp_ptr[2];


  } 
else {
    Move(pos, initial_pos, x_pos, y_pos, temp_ptr);

    
    move_ptr[0] = temp_ptr[0];
    move_ptr[1] = temp_ptr[1];

    move_ptr[2] = temp_ptr[2];
    
    move_ptr[3] = -1;
  }
}

void Move(int *pos, int *initial_pos, double x_pos, double y_pos, double *temp_ptr) {
  int x_move = pos[1] - initial_pos[1];
  int y_move = pos[0] - initial_pos[0];




  double arr[3];
  double x_pos_new = 0;
  double y_pos_new = 0;

  if (x_move > 0) {
    if (y_move > 0) {
      x_pos_new = MoveX(x_move, x_pos);
      arr[0] = x_pos_new;
      y_pos_new = MoveY(y_move, y_pos);
      arr[1] = y_pos_new;
      arr[2] = 0;
    } else {
      y_pos_new = MoveY(y_move, y_pos);
      arr[0] = y_pos_new;
      x_pos_new = MoveX(x_move, x_pos);
      arr[1] = x_pos_new;
      arr[2] = 1;
    }
  } else {
    if (y_move > 0) {
      x_pos_new = MoveX(x_move, x_pos);
      arr[0] = x_pos_new;
      y_pos_new = MoveY(y_move, y_pos);
      arr[1] = y_pos_new;

      arr[2] = 0;
    } else {

      y_pos_new = MoveY(y_move, y_pos);
      arr[0] = y_pos_new;
      x_pos_new = MoveX(x_move, x_pos);
      arr[1] = x_pos_new;
      arr[2] = 1;
    }
  }

  for (int i = 0; i < 3; i ++){
    temp_ptr[i] = arr[i];
  }
}

// Takes in a note and transforms it into the note class
Note Transform_Note(String note_str) {
  char letter = note_str[0];
  char octave = note_str[1];

  int octave_i = octave - '0';

  int temp_num = 0;
  switch (letter) {
  case 'C':
    temp_num = 0;
    break;
  case 'D':
    temp_num = 1;
    break;
  case 'E':
    temp_num = 2;
    break;
  case 'F':
    temp_num = 3;
    break;
  case 'G':
    temp_num = 4;
    break;
  case 'A':
    temp_num = 5;
    break;
  case 'B':
    temp_num = 6;
    break;

  }

  int note_num = temp_num + (octave_i - 1) * 7;

  Note note;
  note.note = note_str;
  note.note_num = note_num;

  return note;
}

// Takes in a note and provides x, y shift
void Note_To_Direction(Note note_start, int *pos) {

  int y_a = 0;
  int x_a = 0;
  if (note_start.note_num%2 != 0){
    y_a = ceil((note_start.note_num+1) / 2);
    x_a = ceil((note_start.note_num + 1) / 2) * pow(-1, note_start.note_num);
  }
  else{
  y_a = ceil(note_start.note_num / 2);
   x_a = ceil(note_start.note_num / 2) * pow(-1, note_start.note_num);
    }

  *(pos) = y_a;
  *(pos + 1) = x_a;

  int arr[] = {y_a, x_a};
  int *note_pos;
  note_pos = arr;
  // return note_pos;
}

double MoveX(int x_move, double x_pos) {
  // left motion
  double x_length = x_move * tab_length;
  double x_rotate = 0;
  if (x_move < 0) {
    //left motion
    x_rotate = Length_To_Rotate(x_length*(-1), x_r, teeth);
    x_pos = x_pos - x_rotate;
  } else {
    // right motion
    x_rotate = Length_To_Rotate(x_length, x_r, teeth);
    x_pos = x_pos + x_rotate;
  }

  return x_pos;
}

double MoveY(int y_move, double y_pos) {
  double y_length = y_move * tab_height;

  double y_rotate = 0;
  if (y_move < 0) {
    // Move Down
    y_rotate = Length_To_Rotate(y_length*(-1), y_r, teeth);
    y_pos = y_pos - y_rotate;
  } else {
    y_rotate = Length_To_Rotate(y_length, y_r, teeth);
    y_pos = y_pos + y_rotate;


  }

  return y_pos;
}

double Length_To_Rotate(double len, double r, double teeth) {
  return len /(r * cos(atan(4)/2 - atan(4) / teeth) * teeth) * 180;
  // returns angle rotation corresponding to the necessary length
}

// Actual Motor stuff

Servo servo_x;
Servo servo_y1;
Servo servo_y2;
Servo servo_hammer;

int pos_x = 0;
int pos_y = 0;

int pos_hammer = 0;

int new_pos_x = 0;
int new_pos_y = 0;

int LED = 2;


int servoPin_x = 9;
int servoPin_y1 = 3;
int servoPin_y2 = 6;
int servoPin_hammer = 12;

void setup(){
  servo_x.attach(servoPin_x);
  servo_y1.attach(servoPin_y1);
  servo_y2.attach(servoPin_y2);
  servo_hammer.attach(servoPin_hammer);
  pinMode(LED, OUTPUT);
}


void loop(){

digitalWrite(LED, HIGH);
//pos_x = 0;

/*
for (pos_x = 0; pos_x < 180; pos_x +=1){
    servo_x.write(pos_x);
    servo_y1.write(pos_x);
    servo_y2.write(pos_x);
    servo_hammer.write(pos_x);
     delay(15);
}

for(pos_x = 180; pos_x > -1; pos_x -=1){
    servo_x.write(pos_x);
    servo_y1.write(pos_x);
    servo_y2.write(pos_x);
    servo_hammer.write(pos_x);
     delay(15);
}
*/

 String notes = "A1F1D1C1";
 
 int splitLength = 2;
 int NumSubstrings = notes.length()/splitLength;
 //double moves[NumSubstrings*6 + 3];
 double* moves = (double*)calloc(NumSubstrings*6 + 3, sizeof(double));

 int num = 3;
 int *count = &num;
 Parse_Notes(notes, moves, count);

double* final_moves = (double*)calloc(num, sizeof(double));
//double final_moves[num];

 for (int i = 0; i < num; i ++){
    *(final_moves + i) = *(moves + i);
  }
  
    for (pos_x; pos_x < 20; pos_x ++){
        servo_x.write(pos_x);
        delay(15);
    }
    for (pos_y; pos_y > 0; pos_y --){
        servo_y1.write(pos_y);
        servo_y2.write(pos_y);
    }
    for (int i = 0; i < num - 3; i ++){
        double x_y = *(final_moves + 3*(i+1) + 2);
        double x_y_past = *(final_moves + 3*i + 2);
        if (x_y == 0){
        //Case 1: Rotate X, Rotate Y, Hit
        //Rotate Servo X
        //Rotate Servo Y
        if (x_y_past == 0){
            pos_x = int(*(final_moves + 3*i));
            pos_y = int(*(final_moves + 3*i + 1));
        }
        else{
          pos_x = int(*(final_moves + 3*i + 1));
          pos_y = int(*(final_moves + 3*i));
        }

        new_pos_x = int(*(final_moves + 3*(i + 1)));
        new_pos_y = int(*(final_moves + 3*(i + 1) + 1));

        //Case x > 0 y > 0
         servo_x.write(new_pos_x);
         servo_y1.write(new_pos_y);
         servo_y2.write(180 - new_pos_y);
         
        if (new_pos_x > pos_x){
          servo_x.write(pos_x);
          /*
            for(pos_x; pos_x < new_pos_x; pos_x ++){
                servo_x.write(pos_x);
                delay(15);
            }
                if (new_pos_y > pos_y){
                    for(pos_y; pos_y < new_pos_y; pos_y ++){
                        servo_y1.write(pos_y);
                        servo_y2.write(pos_y);
                    }
                }
                else{
                    for(pos_y; pos_y > new_pos_y; pos_y --){
                        servo_y1.write(pos_y);
                        servo_y2.write(pos_y);
                    }

                }
                */
        }
        else{
          /*
            for(pos_x; pos_x > new_pos_x; pos_x --){
                servo_x.write(pos_x);
                delay(15);
            }
                if (new_pos_y > pos_y){
                    for(pos_y; pos_y < new_pos_y; pos_y ++){
                        servo_y1.write(pos_y);
                        servo_y2.write(pos_y);
                    }
                }
                else{
                    for(pos_y; pos_y > new_pos_y; pos_y --){
                        servo_y1.write(pos_y);
                        servo_y2.write(pos_y);
                    }
                }
                */

        }

        //Case x < 0 y > 0
        }
        else{
        //Case 2: Rotate Y, Rotate X, Hit
        //Rotate servo Y
        //Rotate Servo X
        //Hit
        if (x_y_past == 0){
            pos_x = int(*(final_moves + 3*i));
            pos_y = int(*(final_moves + 3*i + 1));
        }
        else{
          pos_x = int(*(final_moves + 3*i + 1));
          pos_y = int(*(final_moves + 3*i));
        }

        new_pos_x = int(*(final_moves + 3*(i + 1) + 1));
        new_pos_y = int(*(final_moves + 3*(i + 1)));
        
         servo_x.write(new_pos_x);
         servo_y1.write(new_pos_y);
         servo_y2.write(180-new_pos_y);
         
        if (new_pos_x > pos_x){
          /*
            for(pos_x; pos_x < new_pos_x; pos_x ++){
                servo_x.write(pos_x);
                delay(15);
            }
                if (new_pos_y > pos_y){
                    for(pos_y; pos_y < new_pos_y; pos_y ++){
                        servo_y1.write(pos_y);
                        servo_y2.write(pos_y);
                    }
                }
                else{
                    for(pos_y; pos_y > new_pos_y; pos_y --){
                        servo_y1.write(pos_y);
                        servo_y2.write(pos_y);
                    }

                }
                */
        }
        else{
          /*
          
            for(pos_x; pos_x > new_pos_x; pos_x --){
                servo_x.write(pos_x);
                delay(15);
            }
                if (new_pos_y > pos_y){
                    for(pos_y; pos_y < new_pos_y; pos_y ++){
                        servo_y1.write(pos_y);
                        servo_y2.write(pos_y);
                    }
                }
                else{
                    for(pos_y; pos_y > new_pos_y; pos_y --){
                        servo_y1.write(pos_y);
                        servo_y2.write(pos_y);
                    }
                }
*/
        }

        }
    delay(100);
    for (pos_hammer; pos_hammer <= 180; pos_hammer ++){
      servo_hammer.write(pos_hammer);
    }
    
    for (pos_hammer; pos_hammer > -1; pos_hammer --){
      servo_hammer.write(pos_hammer);
    }
    pos_hammer = 0;
    delay(1000);
    }

}
