word convertRGB( byte R, byte G, byte B);

typedef struct ThemeGUI {
  word primaryColor, secondaryColor, backgroundColor;
  char verboseName[13];
};

ThemeGUI themes[] = {
  {
    convertRGB(255,255,255),
    convertRGB(255,255,255),
    convertRGB(0,0,0),
    "Classic",
  },
  {
    convertRGB(255, 20, 40),
    convertRGB(255, 100, 100),
    convertRGB(0,0,0),
    "Red",
  },
  {
    convertRGB(15, 82, 200),
    convertRGB(130, 130, 255),
    convertRGB(0,0,0),
    "Blue",
  },
  {
    convertRGB(65, 183, 80),
    convertRGB(130, 255, 130),
    convertRGB(0,0,0),
    "Green",
  },
  {
    convertRGB(155, 60, 220),
    convertRGB(225, 40, 225),
    convertRGB(0,0,0),
    "Purple",
  },
  {
    convertRGB(75, 130, 145),
    convertRGB(75, 130, 145),
    convertRGB(0,0,0),
    "Navy Blue",
  },
  {
    convertRGB(0, 0, 0),
    convertRGB(0, 0, 0),
    convertRGB(255,255,255),
    "Negative",
  },
};


const size_t NUM_OF_THEMES = sizeof(themes)/sizeof(*themes);
const size_t SIZE_OF_THEME = sizeof(ThemeGUI);
