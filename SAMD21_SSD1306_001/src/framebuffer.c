#include <SSD1306.h>
#include <framebuffer.h>
#include <bitmap_db.h>
#include "string.h"

const uint8_t LEFT=0;
const uint8_t CENTRE=1;
const uint8_t RIGHT=2;
const uint8_t FREEPOS=3;

const uint8_t EXCLAMATION = 0;
const uint8_t DQUOTE = 1;
const uint8_t HASH = 2;
const uint8_t DOLLAR = 3;
const uint8_t PERCENT = 4;
const uint8_t AMPERSAND = 5;
const uint8_t SQUOTE = 6;
const uint8_t OBRACKET = 7;
const uint8_t CBRACKET = 8;
const uint8_t ASTERISK = 9;
const uint8_t PLUS = 10;
const uint8_t COMMA = 11;
const uint8_t DASH = 12;
const uint8_t DOT = 13;
const uint8_t FSLASH = 14;
const uint8_t ZERO = 15;
const uint8_t COLON = 25;
const uint8_t SCOLON = 26;
const uint8_t LTHAN = 27;
const uint8_t EQUALS = 28;
const uint8_t GTHAN = 29;
const uint8_t QUESTION = 30;
const uint8_t AT = 31;
const uint8_t CAPA = 32;
const uint8_t OSQUARE = 58;
const uint8_t BSLASH = 59;
const uint8_t CSQUARE = 60;
const uint8_t CARET = 61;
const uint8_t USCORE = 62;
const uint8_t OQUOTE = 63;
const uint8_t LOWERA = 64;
const uint8_t OCURL = 90;
const uint8_t PIPE = 91;
const uint8_t CCURL = 92;
const uint8_t TILDE = 93;



void buffer_clear(uint8_t *buffer)
{
  for (uint16_t j = 0; j < 1024; j++) buffer[j]=0;

}

void buffer_drawPixel(uint8_t *buffer, uint8_t pos_x, uint8_t pos_y, uint8_t pixel_status) {
  if (pos_x >= SSD1306_WIDTH || pos_y >= SSD1306_HEIGHT) {
    return;
  }

  if (pixel_status) {
    buffer[pos_x+(pos_y/8)*SSD1306_WIDTH] |= (1 << (pos_y&7));
    } else {
    buffer[pos_x+(pos_y/8)*SSD1306_WIDTH] &= ~(1 << (pos_y&7));
  }
}

void buffer_drawVLine(uint8_t *buffer, uint8_t x, uint8_t y, uint8_t length) {
  for (uint8_t i = 0; i < length; ++i) {
    buffer_drawPixel(buffer, x,i+y,true);
  }
}

void buffer_drawHLine(uint8_t *buffer, uint8_t x, uint8_t y, uint8_t length) {
  for (uint8_t i = 0; i < length; ++i) {
    buffer_drawPixel(buffer, i+x,y,true);
  }
}

void buffer_drawRectangle(uint8_t *buffer, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
  uint8_t length = x2 - x1 + 1;
  uint8_t height = y2 - y1;

  buffer_drawHLine(buffer, x1,y1,length);
  buffer_drawHLine(buffer, x1,y2,length);
  buffer_drawVLine(buffer, x1,y1,height);
  buffer_drawVLine(buffer, x2,y1,height);
}

void buffer_drawBitmap(uint8_t *buffer, const uint8_t *bitmap, uint8_t height, uint8_t width, uint8_t pos_x, uint8_t pos_y) {
  uint8_t current_byte;
  uint8_t byte_width = (width + 7)/8;

  for (uint8_t current_y = 0; current_y < height; current_y++) {
    for (uint8_t current_x = 0; current_x < width; current_x++) {
      current_byte = bitmap[current_y*byte_width + current_x/8];
      if (current_byte & (128 >> (current_x&7))) {
        buffer_drawPixel(buffer, current_x+pos_x,current_y+pos_y,1);
        } else {
        buffer_drawPixel(buffer, current_x+pos_x,current_y+pos_y,0);
      }
    }
  }
}


uint8_t Punctuate(char mark) {
  switch (mark)
  {
    case '!': return EXCLAMATION; break;
    case '"' : return DQUOTE; break;
    case '#' : return HASH; break;
    case '$' : return DOLLAR; break;
    case '%' : return PERCENT; break;
    case '&' : return AMPERSAND; break;
    case '\'' : return SQUOTE; break;
    case '(' : return OBRACKET; break;
    case ')' : return CBRACKET; break;
    case '*' : return ASTERISK; break;
    case '+' : return PLUS; break;
    case ',' : return COMMA; break;
    case '-' : return DASH; break;
    case '.' : return DOT; break;
    case '/' : return FSLASH; break;
    case '0' : return ZERO; break;
    case ':' : return COLON; break;
    case ';' : return SCOLON; break;
    case '<' : return LTHAN; break;
    case '=' : return EQUALS; break;
    case '>' : return GTHAN; break;
    case '?' : return QUESTION; break;
    case '@' : return AT; break;
    case 'A' : return CAPA; break;
    case '[' : return OSQUARE; break;
    case '\\' : return BSLASH; break;
    case ']' : return CSQUARE; break;
    case '^' : return CARET; break;
    case '_' : return USCORE; break;
    case '`' : return OQUOTE; break;
    case 'a' : return LOWERA; break;
    case '{' : return OCURL; break;
      case '|' : return PIPE; break;
    case '}' : return CCURL; break;
    case '~' : return TILDE; break;
  }
  return QUESTION;
}

void buffer_WriteText(uint8_t *buffer, const FONT_INFO *fontp, const char text[], uint8_t x, uint8_t y, uint8_t orient )
{
  
  //uint8_t chars[sizeof(text)];
  uint8_t nChars = (strlen(text));
  uint8_t chars[nChars];
  memset( chars, 0, nChars*sizeof(uint8_t) );  
  
  uint8_t width=0;

  
  //for (uint8_t i=0; i<sizeof(text);i++)
  for (uint8_t i=0; i<nChars;i++)
  {
    if ((uint8_t)text[i]==0x20) width=width+fontp->spacePixels;
    else
    {
      if ((uint8_t)text[i]>=0x41 && (uint8_t)text[i]<=0x5A) chars[i]=(uint8_t)text[i]-0x41+CAPA;
      if ((uint8_t)text[i]>=0x61 && (uint8_t)text[i]<=0x7A) chars[i]=(uint8_t)text[i]-0x61+LOWERA;
      if ((uint8_t)text[i]>=0x30 && (uint8_t)text[i]<=0x39) chars[i]=(uint8_t)text[i]-0x30+ZERO;
      if (chars[i]==0) chars[i]=Punctuate(text[i]);
      width=width+fontp->charInfo[chars[i]].widthBits+2;
    }
  }
  switch(orient)
  {
    case 0: x=0; break;  //LEFT
    case 1: x=64-(width/2); break;  //CENTRE
    case 2: x=128-width; break; // RIGHT
  }
  //for (uint8_t i=0; i<sizeof(text);i++)
  for (uint8_t i=0; i<nChars;i++)
  {
    if ((uint8_t)text[i]==0x20) x=x+fontp->spacePixels;
    else
    {
      buffer_drawBitmap(buffer, fontp->data + fontp->charInfo[chars[i]].offset,fontp->heightPixels,fontp->charInfo[chars[i]].widthBits,x,y);
      x=x+fontp->charInfo[chars[i]].widthBits+2;
    }
  }
  
}

void buffer_WriteTextToConsole(uint8_t *buffer,const FONT_INFO *fontp, const char *text){

  uint8_t nChars = (strlen(text));
  uint8_t chars[nChars];
  memset( chars, 0, nChars*sizeof(uint8_t) );
    
  uint8_t width=0;
  uint8_t nLinesReqd=1;
  uint8_t x=0;
  uint8_t y=0;
  
  // Determine how many lines are required to display text
  for (uint8_t i=0; i<nChars;i++)
  {
    if ((uint8_t)text[i]==0x20) {
      if(width+fontp->spacePixels>128){
        nLinesReqd++;
        width=fontp->spacePixels;
        } else {
        width=width+fontp->spacePixels;
      }
      } else {
      if ((uint8_t)text[i]>=0x41 && (uint8_t)text[i]<=0x5A) chars[i]=(uint8_t)text[i]-0x41+CAPA;
      if ((uint8_t)text[i]>=0x61 && (uint8_t)text[i]<=0x7A) chars[i]=(uint8_t)text[i]-0x61+LOWERA;
      if ((uint8_t)text[i]>=0x30 && (uint8_t)text[i]<=0x39) chars[i]=(uint8_t)text[i]-0x30+ZERO;
      if (chars[i]==0) chars[i]=Punctuate(text[i]);
      if(width+fontp->charInfo[chars[i]].widthBits+2>128) {
        nLinesReqd++;
        width=fontp->charInfo[chars[i]].widthBits+2;
        } else {
        width=width+fontp->charInfo[chars[i]].widthBits+2;
      }
    }
  }
  
  // Shift display up by that number of rows
  scrollRows(buffer, nLinesReqd*fontp->heightPixels);
  y=64-(nLinesReqd*fontp->heightPixels);
  
  // write text into buffer
  for (uint8_t i=0; i<nChars;i++)
  {
    if ((uint8_t)text[i]==0x20) {
      if(x+fontp->spacePixels>128) {
        x=fontp->spacePixels;
        y=y+fontp->heightPixels;
      } else {
        x=x+fontp->spacePixels;
      }
    }
    else
    {
      if(x+fontp->charInfo[chars[i]].widthBits+2>128) {
        x=0;
        y=y+fontp->heightPixels;
      }
      
      buffer_drawBitmap(buffer, fontp->data +fontp->charInfo[chars[i]].offset,fontp->heightPixels,fontp->charInfo[chars[i]].widthBits,x,y);
      x=x+fontp->charInfo[chars[i]].widthBits+2;
      
    }
  }
  
  
}

void scrollRows(uint8_t *buffer, uint8_t rows) {
  
  uint8_t pages;
  pages=rows / 8;
  if(rows>=8) scrollPage(buffer, pages); // use scrollPage to do most of the work
  
  rows=rows%8; // how many rows are left to shift?
  
  for (uint8_t i=0; i<128; i++) {  // for each column
    for (uint8_t j=0; j<8-pages; j++) { // and for each page
      
      buffer[(j*128)+i] = (buffer[(j*128)+i]>>rows) | (buffer[(j+1)*128+i]<<(8-rows));
      
    }
  }
  
}

void scrollPage(uint8_t *buffer, uint8_t pages) {
  
  memmove(buffer,buffer+pages*128,(8-pages)*128);
  for(int i=0; i<128*pages; i++) buffer[(8-pages)*128+i]=0;
}

void Byte2String (char* outstr ,uint8_t u8t)
{
  
  if((uint8_t)u8t/100==0){
    outstr[0]=0x20;
    } else {
    outstr[0]=0x30 + (uint8_t)u8t/100;
  }
  
  if((uint8_t)u8t/100==0 && (uint8_t)(u8t%100)/10==0){
    outstr[1]=0x20;
    } else {
    outstr[1]= 0x30 + (uint8_t)(u8t%100)/10;
  }
  
  outstr[2]=0x30 + (uint8_t)(u8t%10);
}

void buffer_drawButtons(uint8_t *buffer) 
{
	buffer_clear(buffer);
	
	buffer[0]=0xFE;
	buffer[15]=0xFE;
	
	//
	//for (uint8_t i=1, i<15, i++) 
	//{
////		buffer[i]
		//
	//}
	
}
