#ifndef URG_SERIAL_UTILS_H
#define URG_SERIAL_UTILS_H



extern int urg_serial_find_port(void);


extern const char *urg_serial_port_name(int index);


/**
  \brief

  \retval 1 URG
  \retval 0
  \retval <0
*/
extern int urg_serial_is_urg_port(int index);

#endif /* !URG_SERIAL_UTILS_H */
