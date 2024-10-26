typedef enum {
    SETTING_ENUMERATED,
    SETTING_INT,
    SETTING_UINT,
} _setting_type;

typedef struct {
    const char *name;
    const char *setting_values[];
 } _setting_def;
 /*
 {
    "Open Common Polarity",
    {"Positive", "Negative"}
 }
 {
    "Close Common Polarity",
    {"Positive", "Negative"}
 }
 {
    "Local Fan Follows:",
    {"Global Fan", "Local Open"}
 }
 {
    "Beep Volume",
    {"High", "Medium", "Low", "Off"}
 }
 {
    "Rotation",
    {"Normal", "Inverted"}
}
 {
    "Inactivity Timeout",
    {"Off", "5 min", "10 min", "15 min", "30 min", "60 min", "120 min", "240 min", "480 min"}
 }
  {
    "Inactivity Warning",
    {"Off", "10 sec", "30 sec", "60 sec", "90 sec", "2 min", "5 min", "10 min", "15 min"}
 }
   {
    "Inactivity Warning On",
    {"Always", "Only When Open"}
 }
 {
     "Shutdown state",
     {"Open", "Close", "Keep last"}
 }
 {
    "Station Number",
    {}
 }
  {
    "Green LED Brightness",
    {}
 }
  {
    "Red LED Brightness",
    {}
 }
 {
    "Open Servo Position",
    {}
 }
 {
    "Close Servo Position",
    {}
 }
 */
