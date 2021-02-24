#include <stdio.h>
#include <webots/keyboard.h>
#include <webots/motor.h>
#include <webots/robot.h>
#include <webots/distance_sensor.h>
#include <conio.h>
#include <math.h>
#include <time.h>

#define TIME_STEP 64
#define VELOCITY 0.6

enum {
  wheel2servo,
  wheel4servo,
  wheel6servo,
  wheel2,
  wheel4,
  wheel6,
  wheel1servo,
  wheel3servo,
  wheel5servo,
  wheel1,
  wheel3,
  wheel5,
  ds_left_front,
  ds_right_front,
  ds_left,
  ds_right,
  JOINTS_MAX
};

WbDeviceTag joints[JOINTS_MAX];

void move_4_wheels(double v) {
  wb_motor_set_velocity(joints[wheel2], v * VELOCITY);
  wb_motor_set_velocity(joints[wheel4], v * VELOCITY);
  wb_motor_set_velocity(joints[wheel6], v * VELOCITY);
  wb_motor_set_velocity(joints[wheel1], v * VELOCITY);
  wb_motor_set_velocity(joints[wheel3], v * VELOCITY);
  wb_motor_set_velocity(joints[wheel5], v * VELOCITY);

  wb_motor_set_available_torque(joints[wheel3], 0.0);
  wb_motor_set_available_torque(joints[wheel4], 0.0);
}

void move_6_wheels(double v) {
  wb_motor_set_available_torque(joints[wheel3], 2.0);
  wb_motor_set_available_torque(joints[wheel4], 2.0);

  wb_motor_set_velocity(joints[wheel2], v * VELOCITY);
  wb_motor_set_velocity(joints[wheel4], v * VELOCITY);
  wb_motor_set_velocity(joints[wheel6], v * VELOCITY);
  wb_motor_set_velocity(joints[wheel1], v * VELOCITY);
  wb_motor_set_velocity(joints[wheel3], v * VELOCITY);
  wb_motor_set_velocity(joints[wheel5], v * VELOCITY);
}

void turn_wheels_right() {
  wb_motor_set_position(joints[ wheel2servo], 0.4);
  wb_motor_set_position(joints[wheel1servo], 0.227);
  wb_motor_set_position(joints[wheel5servo], -0.227);
  wb_motor_set_position(joints[wheel6servo], -0.4);
}

void turn_wheels_left() {
  wb_motor_set_position(joints[ wheel2servo], -0.227);
  wb_motor_set_position(joints[wheel1servo], -0.4);
  wb_motor_set_position(joints[wheel5servo], 0.4);
  wb_motor_set_position(joints[wheel6servo], 0.227);
}

void wheels_straight() {
  wb_motor_set_position(joints[ wheel2servo], 0.0);
  wb_motor_set_position(joints[wheel1servo], 0.0);
  wb_motor_set_position(joints[wheel5servo], 0.0);
  wb_motor_set_position(joints[wheel6servo], 0.0);
}

void turn_around(double v) {
  wb_motor_set_position(joints[ wheel2servo], -0.87);
  wb_motor_set_position(joints[wheel1servo], 0.87);
  wb_motor_set_position(joints[wheel5servo], -0.87);
  wb_motor_set_position(joints[wheel6servo], 0.87);

  wb_motor_set_velocity(joints[wheel2], v * VELOCITY);
  wb_motor_set_velocity(joints[wheel4], v * VELOCITY);
  wb_motor_set_velocity(joints[wheel6], v * VELOCITY);
  wb_motor_set_velocity(joints[wheel1], -v * VELOCITY);
  wb_motor_set_velocity(joints[wheel3], -v * VELOCITY);
  wb_motor_set_velocity(joints[wheel5], -v * VELOCITY);

  wb_motor_set_available_torque(joints[wheel3], 0.0);
  wb_motor_set_available_torque(joints[wheel4], 0.0);
}

int main() {

  float  avoid_obstacle_counter1 = 0;
  int  avoid_obstacle_counter2 = 0;
  int  avoid_obstacle_counter3 = 0;
  int  avoid_obstacle_counter4 = 0;
  int wall_left = 0;
  int wall_right = 0;
  int explore = 0;
  double ds_values[4];
  int key = 'N';
  int key2 = 'N';
  
  // Required to initialize Webots
  wb_robot_init();
  
  joints[wheel2servo] = wb_robot_get_device("wheel2servo");
  joints[wheel6servo] = wb_robot_get_device("wheel6servo");
  
  joints[wheel2] = wb_robot_get_device("wheel2");
  joints[wheel4] = wb_robot_get_device("wheel4");
  joints[wheel6] = wb_robot_get_device("wheel6");

  joints[wheel1servo] = wb_robot_get_device("wheel1servo");
  joints[wheel5servo] = wb_robot_get_device("wheel5servo");
  
  joints[wheel1] = wb_robot_get_device("wheel1");
  joints[wheel3] = wb_robot_get_device("wheel3");
  joints[wheel5] = wb_robot_get_device("wheel5");
  
  joints[ds_left_front] = wb_robot_get_device("ds_left_front");
  joints[ds_right_front] = wb_robot_get_device("ds_right_front");
  
  joints[ds_left] = wb_robot_get_device("ds_left");
  joints[ds_right] = wb_robot_get_device("ds_right");

  wb_distance_sensor_enable(joints[ds_left_front], TIME_STEP);
  wb_distance_sensor_enable(joints[ds_right_front], TIME_STEP);
  
  wb_distance_sensor_enable(joints[ds_left], TIME_STEP);
  wb_distance_sensor_enable(joints[ds_right], TIME_STEP);

  wb_motor_set_position(joints[wheel2], INFINITY);
  wb_motor_set_position(joints[wheel4], INFINITY);
  wb_motor_set_position(joints[wheel6], INFINITY);
  wb_motor_set_position(joints[wheel1], INFINITY);
  wb_motor_set_position(joints[wheel3], INFINITY);
  wb_motor_set_position(joints[wheel5], INFINITY);
  
  printf("Welcome to ExoMy Simulation\n");
  printf("Select the 3D window and use the keyboard to drive this robot:\n");
  printf("\n");
  printf("Q: forwards-left;  W: forwards;  E: forwards-right\n");
  printf("S: spin counter-clockwise\n");
  printf("Y: backwards-left; X: backwards; C: backwards-right\n");
  printf("H: Set explore mode! Press any other key to exit\n");
  printf("P: Stop the robot\n");
  
  wb_keyboard_enable(TIME_STEP);

  // start moving
  move_6_wheels(5.0);

  while (wb_robot_step(TIME_STEP) != -1) {
  
     key = wb_keyboard_get_key(); 
     
     if (key == 'W' || key == 'X' || key == 'Q' || key == 'E' || key == 'S' || key == 'Z' || key == 'C' || key == 'P') {
     
          explore = 0;
          key2 = key; 
          
      } else if  ( key2 == 'H') {
      
          explore = 1;   
          
      } else if ( key == 'H'){
      
          explore = 1;
          key2 = 'H'; 
          printf("You have entered explore mode\n");     
          
      } 
      
    switch (key2) {
    
          case 'W':
            // forwards
            wheels_straight();
            move_6_wheels(5.0);
          break;
          
          case 'X':
            // backwards
            wheels_straight();
            move_6_wheels(-5.0);
          break;
          
          case 'Q':
            // forwards left
            turn_wheels_right();
            move_4_wheels(5.0);
          break;
          
          case 'E':
            // forwards right
            turn_wheels_left();
            move_4_wheels(5.0);
          break;
          
          case 'Z':
            // backwards left
            turn_wheels_right();
            move_4_wheels(-5.0);
          break;
          
          case 'C':
            // backwards right
            turn_wheels_left();
            move_4_wheels(-5.0);
          break;
        
          case 'S':
            // spin counter-clockwise
            turn_around(5.0);
          break;
        
          case 'P':
            // stop robot
            wheels_straight();
            move_6_wheels(0.0);
          break;
     }
        
      
    // printf("explore = %d\n", explore);
    
     if (explore == 0){
     
          avoid_obstacle_counter1 = 0;
          avoid_obstacle_counter2 = 0;
          avoid_obstacle_counter3 = 0;
          avoid_obstacle_counter4 = 0;
          wall_left = 0;
          wall_right = 0;
     }
     
     
     if (explore == 1){
       
          ds_values[0] = wb_distance_sensor_get_value(joints[ds_left_front]);
          ds_values[1] = wb_distance_sensor_get_value(joints[ds_right_front]);
                        
          ds_values[2] = wb_distance_sensor_get_value(joints[ds_left]);
          ds_values[3] = wb_distance_sensor_get_value(joints[ds_right]);
            
          // printf("%f %f %f %f\n",ds_values[0], ds_values[1], ds_values[2], ds_values[3] );
                        
          if ((ds_values[0] < 15.0 || ds_values[1] < 15.0) && avoid_obstacle_counter1 == 0){
          
              avoid_obstacle_counter1 = 37;
              avoid_obstacle_counter2 = 0;
              printf("Wall detected in front!\n");
              fflush(stdout);
          }
                              
          if (ds_values[2] < 15  && avoid_obstacle_counter3 == 0){
                             
              avoid_obstacle_counter3 = 36; 
              printf("Wall detected to close to the left!\n");
              fflush(stdout);  
          }
                 
          if (ds_values[3] < 15  && avoid_obstacle_counter4 == 0) {
                             
               avoid_obstacle_counter4 = 36;   
               printf("Wall detected to close to the right!\n");
               fflush(stdout); 
          }           
                                             
          if (ds_values[2] < 50)
          
              wall_left = 1;   
                                           
          if (ds_values[3] < 50)
                             
              wall_right = 1;  
                               
                              
          if (ds_values[2] > 50)
                             
              wall_left = 0;   
                                            
          if (ds_values[3] > 50)
                             
              wall_right = 0;  
                                             
          //  printf("Wall right : %d, Wall left: %d\n", wall_right, wall_left);
            
          if (avoid_obstacle_counter1 > 0){
            
              if (avoid_obstacle_counter2 != 0){  
              
                  wheels_straight();
                  move_6_wheels(-5.0);   
                  avoid_obstacle_counter2 = avoid_obstacle_counter2 - 1;
                   
              } else if (wall_left == 1 && wall_right == 0) {
                
                  turn_around(5.0);
                  avoid_obstacle_counter1 = avoid_obstacle_counter1 - 1;
                  
              } else if (wall_left == 0 && wall_right == 1) {
                
                  turn_around(-5.0);
                  avoid_obstacle_counter1 = avoid_obstacle_counter1 - 1;
                  
              } else if (wall_left == 0 && wall_right == 0) {
                
                  turn_around(5.0);
                  avoid_obstacle_counter1 = avoid_obstacle_counter1 - 1;
                  
              } else if (wall_left == 1 && wall_right == 1) {

                  turn_around(5.0);
                  avoid_obstacle_counter1 = avoid_obstacle_counter1 - 1;
                 
              }
               
          } else if (avoid_obstacle_counter3 > 0) {
            
              if (avoid_obstacle_counter3 > 18)   {   
                  turn_wheels_left();
                  move_4_wheels(5.0);
                  avoid_obstacle_counter3 = avoid_obstacle_counter3 - 1;
                  
              } else {
                  turn_wheels_right();
                  move_4_wheels(5.0);
                  avoid_obstacle_counter3 = avoid_obstacle_counter3 - 1;
                  
              }
                   
          } else if (avoid_obstacle_counter4 > 0) {
            
              if (avoid_obstacle_counter4 > 18)   {    
                 turn_wheels_right();
                 move_4_wheels(5.0);
                 avoid_obstacle_counter4 = avoid_obstacle_counter4 - 1;
                  
              } else {
                 turn_wheels_left();
                 move_4_wheels(5.0);
                 avoid_obstacle_counter4 = avoid_obstacle_counter4 - 1;
                  
              } 
             
           } else {
             
              wheels_straight();
              move_6_wheels(5.0);
            
           }
      }                     
  }

  wb_robot_cleanup();

  return 0;
}
