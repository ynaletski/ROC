extern void copy_data(unsigned int, 
                      unsigned char *, 
                      unsigned char *);
extern void get_config(void);
extern void get_pt_type_pos(void);
extern void log_param(unsigned char *,
                      unsigned char *,
                      int,
                      int,
                      int,
                      int,
                      unsigned char);
extern void log_event(int,
                      int,
                      unsigned char *);
extern void log_alarm(int,
                      int,
                      unsigned char *);
extern unsigned char *get_tlp (unsigned char *, 
                               unsigned char *,
                               unsigned char *);
extern void a_euout(int,
                    int,
                    float);
extern void do_euout(int,
                     float);
extern void ee_restore(unsigned char);
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
extern struct db_history_data_def *db_hist_addr (unsigned char);
extern int new_point_type(unsigned char, 
                          unsigned char, 
                          unsigned int, 
                          unsigned char *);
extern int set_template(unsigned char, 
                        unsigned char, 
                        unsigned char *, 
                        unsigned char *);
