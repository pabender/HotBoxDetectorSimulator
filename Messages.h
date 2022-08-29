//The following are message string arrays used repeatedly

// Welcome String "Hotbox Detector Milepost"
char welcome[] = { 20, 96, /* volume setting */
                   21, 114,/* speed  setting */
                   22, 88, /* pitch setting */ 
                   23, 5, /* bend setting */
                   183,136,191,4, /*HOT*/
                   171,7,135,14,194,7,187,4, /*BOX*/
                   8,174,8,128,8,191,8,131,8,194,8,191,153,4, /* DETECTOR */
                   7,140,7,135,7,155,8,148,4,199,137,7,164,187,191,4, /*MILEPOST */
                   '\0'};

// "No Defects"
char no_defects[]={ 20, 96, /* volume setting */
                   21, 114,/* speed  setting */
                   22, 88, /* pitch setting */ 
                   23, 5, /* bend setting */
                   142,164,4, /* NO */
                   174,128,186,196,131,191,187,4, /*defects*/ 
                   '\0'};
   
// "Detector Out"                   
char detector_out[] = { 20, 96, /* volume setting */
                   21, 114,/* speed  setting */
                   22, 88, /* pitch setting */ 
                   23, 5, /* bend setting */
                   8,174,8,128,8,191,8,131,8,194,8,191,153,4, /* DETECTOR */
                   163,191,4, /*0ut*/
                   '\0'};
           
           // "Speed"
char speed_string[]={ 20, 96, /* volume setting */
               21, 114,/* speed  setting */
               22, 88, /* pitch setting */ 
               23, 5, /* bend setting */
               187,198,128,128,176,
             '\0'};

char alarm_string[]={ 20, 96, /* volume setting */
               21, 114,/* speed  setting */
               22, 88, /* pitch setting */ 
               23, 5, /* bend setting */
               8,217, /* play alarm 7 slowly */
               8,217, /* play alarm 7 slowly */
               174,128,186,196,131,191,4, /*defect*/
               8,174,8,128,8,191,8,131,8,191,8,129,176,4, /* DETECTED */
             '\0'};
