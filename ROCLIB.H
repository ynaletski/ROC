extern void copy_data(unsigned int, 
                      unsigned char *, 
                      unsigned char *);
extern void get_config(void);
extern void get_pt_type_pos(void);
extern void log_param(unsigned char *,
                      unsigned char *,
                      unsigned char,
                      unsigned char,
                      unsigned char,
                      unsigned char);
extern void log_event(unsigned int *,
                      unsigned char *);
extern void log_alarm(unsigned char *,
                      unsigned char *);
extern void do_euout(unsigned char,
                     float);
extern void a_euout(unsigned char,
                    unsigned char,
                    float);
extern void set_time(unsigned char *);
extern void get_time(unsigned char *);
extern int get_param(unsigned int, 
                     unsigned int, 
                     unsigned int, 
                     unsigned int, 
                     unsigned char *);
extern int set_param(unsigned int, 
                     unsigned int, 
                     unsigned int, 
                     unsigned int, 
                     unsigned char *);
extern int set_log_param(unsigned char, 
                         unsigned char, 
                         unsigned char, 
                         unsigned char *);
extern int get_minute(unsigned char, 
                      unsigned char, 
                      unsigned int, 
                      unsigned char *);
extern int get_history(unsigned char, 
                       unsigned char, 
                       unsigned int, 
                       unsigned int, 
                       unsigned char *);
extern int get_daily(unsigned char, 
                     unsigned char, 
                     unsigned int, 
                     unsigned int, 
                     unsigned char *);
extern int set_history(unsigned char, 
                       unsigned char, 
                       unsigned int, 
                       unsigned int, 
                       unsigned char *);
extern int set_daily(unsigned char, 
                     unsigned char, 
                     unsigned int, 
                     unsigned int, 
                     unsigned char *);
extern int get_alarms(unsigned char, 
                      unsigned char, 
                      unsigned char *);
extern int get_events(unsigned char, 
                      unsigned char, 
                      unsigned char *);
extern float *db_data_addr (unsigned char ,
                            unsigned char,
                            unsigned int);
extern int new_point_type(unsigned char, 
                          unsigned char, 
                          unsigned int, 
                          unsigned char *);
extern int set_template(unsigned char, 
                        unsigned char, 
                        unsigned char *, 
                        unsigned char *);
extern int check_change(unsigned char, 
                        unsigned char, 
                        unsigned char, 
                        unsigned char); 
