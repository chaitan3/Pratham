/**
 *@file  frame.c
 *@brief Implements Frame Transformations
*/
#include "common.h"
#include "frame.h"
#include "peripherals.h"

uint64_t seconds_since_equinox, seconds_since_pivot;

uint64_t days_in_months(uint8_t month)
{
  switch (month)
  {
    case 1:
      return 0;
      break;
    case 2:
      return 31;
      break;
    case 3:
      return 59;
      break;
    case 4:
      return 90;
      break;
    case 5:
      return 120;
      break;
    case 6:
      return 151;
      break;
    case 7:
      return 181;
      break;
    case 8:
      return 212;
      break;
    case 9:
      return 243;
      break;
    case 10:
      return 273;
      break;
    case 11:
      return 304;
      break;
    case 12:
      return 334;
      break;
  }
  return 0;
}
uint64_t get_seconds_since_pivot(uint16_t year, uint8_t month, uint8_t date, uint8_t hours, uint8_t minutes, uint8_t seconds)
{
  uint64_t now = 0;
  
  now += ((uint64_t)year - 2010) * 365 * 24 * 60 * 60;
  now += ((uint64_t)date + days_in_months(month) - 1) * 24 * 60 * 60;
  now += ((uint64_t)hours) * 60 * 60;
  now += ((uint64_t)minutes) * 60;
  now += (uint64_t)seconds;
  return now;
}

void get_seconds_since_equinox(void)
{
  uint64_t equinox;
  
  struct GPS_reading * gps = (struct GPS_reading *)&(Current_state.gps);
  
  equinox = get_seconds_since_pivot(2010, 3, 20, 17, 32, 0);
  seconds_since_pivot = get_seconds_since_pivot(gps->year, gps->month, gps->date, gps->hours, gps->minutes, gps->seconds);
  
  seconds_since_equinox = seconds_since_pivot - equinox;
}

void ecef2eci(vector v_eci, vector v_ecef)
{
  float sidereal_time = STPERUT * seconds_since_equinox;
  float phi = W_EARTH_ROT * sidereal_time;
  
  matrix m_rot = { { cos(phi), sin(phi), 0 },
                   { -sin(phi), cos(phi), 0 },
                   { 0, 0, 1 } };
               
  vector_into_matrix(v_eci, m_rot, v_ecef);
}

void eci2ecef(vector v_ecef, vector v_eci)
{
  float sidereal_time = STPERUT * seconds_since_equinox;
  float phi = W_EARTH_ROT * sidereal_time;
  
  matrix m_rot = { { cos(phi), -sin(phi), 0 },
                   { sin(phi), cos(phi), 0 },
                   { 0, 0, 1 } };
               
  vector_into_matrix(v_ecef, m_rot, v_eci);
}

void eci2orbit(vector v_r, vector v_v, vector v_eci, vector v_orbit)
{
  vector v_o_x, v_o_y, v_o_z;
  uint8_t i;
  
  vector_cross_product(v_v, v_r, v_o_y);
  convert_unit_vector(v_o_y);
  
  for(i = 0; i < 3; i++)
    v_o_z[i] = -1 * v_r[i];
  convert_unit_vector(v_o_z);
  
  vector_cross_product(v_o_y, v_o_z, v_o_x);
  convert_unit_vector(v_o_x);
  
  matrix m_o = { { v_o_x[0], v_o_y[0], v_o_z[0] },
                 { v_o_x[1], v_o_y[1], v_o_z[1] },
                 { v_o_x[2], v_o_y[2], v_o_z[2] } };
                 
  vector_into_matrix(v_eci, m_o, v_orbit);
}

void ecef2lla(vector v_ecef, vector v_lla)
{
  float x = v_ecef[0], y = v_ecef[1], z = v_ecef[2];
  float lambda, phi, h, rho, beta, n, sinphi;
  uint8_t i;
  
  lambda = atan2(y, x);
  rho = sqrt(x * x + y * y);
  
  beta = atan2(z, (1 - F) * rho);
  for(i = 0; i < 5; i++)
  {
    phi = atan2(z + B * EP2 * pow(sin(beta), 3), rho - A * E2 * pow(cos(beta), 3));
    beta = atan2((1 - F) * sin(phi), cos(phi));
  }
  sinphi = sin(phi);
  n = A / sqrt(1 - E2 * sinphi);
  h = rho * cos(phi) + (z + E2 * sinphi) * sinphi - n;
  
  v_lla[0] = phi;
  v_lla[1] = lambda;
  v_lla[2] = h;
}

void ned2ecef(vector v_ned, vector v_lla, vector v_ecef)
{
  float clat = cos(v_lla[0]), clon = cos(v_lla[1]);
  float slat = sin(v_lla[0]), slon = sin(v_lla[1]);
  vector v_north, v_east, v_down;
  
  v_north[0] = -1 * slat * clon;
  v_north[1] = -1 * slat * slon;
  v_north[2] = clat;
  
  v_east[0] = -1 * slon;
  v_east[1] = clon;
  v_east[2] = 0;
  
  v_down[0] = -1 * clat * clon;
  v_down[1] = -1 * clat * slon;
  v_down[2] = -1 * slat;
  
  matrix m_ecef = { { v_north[0], v_north[1], v_north[2] },
                    { v_east[0], v_east[1], v_east[2] },
                    { v_down[0], v_down[1], v_down[2] } };
  
  vector_into_matrix(v_ned, m_ecef, v_ecef);
}
