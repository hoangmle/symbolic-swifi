#include "error_lib.h"

int Cur_Vertical_Sep;
int High_Confidence;
int Two_of_Three_Reports_Valid;
int Own_Tracked_Alt;
int Own_Tracked_Alt_Rate;
int Other_Tracked_Alt;
int Alt_Layer_Value;
int Positive_RA_Alt_Thresh_0;
int Positive_RA_Alt_Thresh_1;
int Positive_RA_Alt_Thresh_2;
int Positive_RA_Alt_Thresh_3;
int Up_Separation;
int Down_Separation;
int Other_RAC;
int Other_Capability;
int Climb_Inhibit;

int Own_Below_Threat()
{
  return (Own_Tracked_Alt < Other_Tracked_Alt);
}

int Own_Above_Threat()
{
  return (Other_Tracked_Alt < Own_Tracked_Alt);
}

int initialize()
{
  High_Confidence = 32767;
  Other_Capability = 0;

  Climb_Inhibit = 1;
  Cur_Vertical_Sep = 16684;
  Own_Tracked_Alt_Rate = 450 ;

  CHECK(Own_Tracked_Alt >= 0);
  CHECK(Other_Tracked_Alt >= 0);
  CHECK(Own_Tracked_Alt_Rate >= 0);

  Positive_RA_Alt_Thresh_0 = 16434;
  Positive_RA_Alt_Thresh_1 = 0;
  Positive_RA_Alt_Thresh_2 = 0;
  Positive_RA_Alt_Thresh_3 = 0;
  return 1;
}

int Positive_RA_Alt_Thresh(int Alt)
{
  int res = 0;
  if( Alt == 0 )
    { res = Positive_RA_Alt_Thresh_0; }
  if( Alt == 1 )
    { res = Positive_RA_Alt_Thresh_1; }
  if( Alt == 2 )
    { res = Positive_RA_Alt_Thresh_2; }
  if( Alt == 3 )
    { res = Positive_RA_Alt_Thresh_3; }
  return(res);
}

int ALIM ()
{
  return Positive_RA_Alt_Thresh(Alt_Layer_Value);
}

int Inhibit_Biased_Climb ()
{
  return (Climb_Inhibit ? Up_Separation + 100  : Up_Separation);
}

int Non_Crossing_Biased_Climb()
{
  int upward_preferred;
  int upward_crossing_situation;
  int result;

  upward_preferred = Inhibit_Biased_Climb() > Down_Separation;
  if (upward_preferred)
    {
      result = !(Own_Below_Threat()) || ((Own_Below_Threat()) && (!(Down_Separation >= ALIM())));
    }
  else
    {
      result = Own_Above_Threat() && (Cur_Vertical_Sep >= 300 ) && (Up_Separation >= ALIM());
    }
  return result;
}

int Non_Crossing_Biased_Descend()
{
  int upward_preferred;
  int upward_crossing_situation;
  int result;

  upward_preferred = Inhibit_Biased_Climb() > Down_Separation;
  if (upward_preferred)
    {
      result = Own_Below_Threat() && (Cur_Vertical_Sep >= 300 ) && (Down_Separation >= ALIM());
    }
  else
    {
      result = !(Own_Above_Threat()) || ((Own_Above_Threat()) && (Up_Separation >= ALIM()));
    }
  return result;
}


int alt_sep_test()
{
  int enabled, tcas_equipped, intent_not_known;
  int need_upward_RA = 0;
  int need_downward_RA = 0;

  int alt_sep;

  CHECK(Own_Tracked_Alt >= 300);
  CHECK(Other_Tracked_Alt >= 300);
  CHECK(Own_Tracked_Alt_Rate >= 300);
  
  start_error_injection();

  enabled = High_Confidence && (Own_Tracked_Alt_Rate <= 600 ) && (Cur_Vertical_Sep > 600 );

  tcas_equipped = Other_Capability == 1 ;
  intent_not_known = Two_of_Three_Reports_Valid && Other_RAC == 0 ;

  alt_sep = 0 ;

  if (enabled && ((tcas_equipped && intent_not_known) || !tcas_equipped))
    {
      need_upward_RA = Non_Crossing_Biased_Climb() && Own_Below_Threat() ;
      need_downward_RA = Non_Crossing_Biased_Descend() && Own_Above_Threat();

      if(need_upward_RA && need_downward_RA)
        {
          alt_sep = 0 ;
        }
      else
        if (need_upward_RA)
          {
            alt_sep = 1 ;

          }
        else if (need_downward_RA)
          {
            alt_sep = 2 ;
          }
        else
          alt_sep = 0 ;
    }
    
  stop_error_injection();

  return alt_sep;
}


int main() {
  error_lib_initialize();

  int res1;
  int res2;


  // SAME INPUT for both runs
  
  int i_Cur_Vertical_Sep = klee_int("Cur_Vertical_Sep");
  int i_High_Confidence = klee_int("High_Confidence");
  int i_Two_of_Three_Reports_Valid = klee_int("Two_of_Three_Reports_Valid");;
  int i_Own_Tracked_Alt = klee_int("Own_Tracked_Alt");
  int i_Own_Tracked_Alt_Rate = klee_int("Own_Tracked_Alt_Rate");
  int i_Other_Tracked_Alt = klee_int("Other_Tracked_Alt");
  int i_Alt_Layer_Value = klee_int("Alt_Layer_Value");
  int i_Positive_RA_Alt_Thresh_0 = klee_int("Positive_RA_Alt_Thresh_0");
  int i_Positive_RA_Alt_Thresh_1 = klee_int("Positive_RA_Alt_Thresh_1");
  int i_Positive_RA_Alt_Thresh_2 = klee_int("Positive_RA_Alt_Thresh_2");
  int i_Positive_RA_Alt_Thresh_3 = klee_int("Positive_RA_Alt_Thresh_3");
  int i_Up_Separation = klee_int("Up_Separation");
  int i_Down_Separation = klee_int("Down_Separation");
  int i_Other_RAC = klee_int("Other_RAC");
  int i_Other_Capability = klee_int("Other_Capability");
  int i_Climb_Inhibit = klee_int("Climb_Inhibit");
  
  
  // WITH error injection

  Cur_Vertical_Sep = i_Cur_Vertical_Sep;
  High_Confidence = i_High_Confidence;
  Two_of_Three_Reports_Valid = i_Two_of_Three_Reports_Valid;
  Own_Tracked_Alt = i_Own_Tracked_Alt;
  Own_Tracked_Alt_Rate = i_Own_Tracked_Alt_Rate;
  Other_Tracked_Alt = i_Other_Tracked_Alt;
  Alt_Layer_Value = i_Alt_Layer_Value;
  Positive_RA_Alt_Thresh_0 = i_Positive_RA_Alt_Thresh_0;
  Positive_RA_Alt_Thresh_1 = i_Positive_RA_Alt_Thresh_1;
  Positive_RA_Alt_Thresh_2 = i_Positive_RA_Alt_Thresh_2;
  Positive_RA_Alt_Thresh_3 = i_Positive_RA_Alt_Thresh_3;
  Up_Separation = i_Up_Separation;
  Down_Separation = i_Down_Separation;
  Other_RAC = i_Other_RAC;
  Other_Capability = i_Other_Capability;
  Climb_Inhibit = i_Climb_Inhibit;
  
  initialize();
  
  injection_active = true;
  res1 = alt_sep_test();
  

  // WITHOUT error injection
  
  injection_active = false;
  
  Cur_Vertical_Sep = i_Cur_Vertical_Sep;
  High_Confidence = i_High_Confidence;
  Two_of_Three_Reports_Valid = i_Two_of_Three_Reports_Valid;
  Own_Tracked_Alt = i_Own_Tracked_Alt;
  Own_Tracked_Alt_Rate = i_Own_Tracked_Alt_Rate;
  Other_Tracked_Alt = i_Other_Tracked_Alt;
  Alt_Layer_Value = i_Alt_Layer_Value;
  Positive_RA_Alt_Thresh_0 = i_Positive_RA_Alt_Thresh_0;
  Positive_RA_Alt_Thresh_1 = i_Positive_RA_Alt_Thresh_1;
  Positive_RA_Alt_Thresh_2 = i_Positive_RA_Alt_Thresh_2;
  Positive_RA_Alt_Thresh_3 = i_Positive_RA_Alt_Thresh_3;
  Up_Separation = i_Up_Separation;
  Down_Separation = i_Down_Separation;
  Other_RAC = i_Other_RAC;
  Other_Capability = i_Other_Capability;
  Climb_Inhibit = i_Climb_Inhibit;
  
  initialize();
  
  res2 = alt_sep_test();


  // CHECK result
  
  assert (res1 == res2);  
  
  return 0;
}
