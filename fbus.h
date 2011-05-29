enum frametype fbus_readframe(uint8_t timeout);
uint8_t fbus_sendsms(const char *num, const char *msg);
uint8_t fbus_heartbeat(void);

extern char unbcd_buf[16];
extern char unpack7_buf[32];

enum frametype {
    FRAME_READ_TIMEOUT,
    FRAME_SMS_SENT,
    FRAME_SMS_RECV,
    FRAME_SMS_ERROR,
    FRAME_ACK,
    FRAME_ID,
    FRAME_NET_STATUS,
    FRAME_UNKNOWN,
};