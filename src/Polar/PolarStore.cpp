/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2011 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "Polar/PolarStore.hpp"
#include "Polar/Polar.hpp"

#include <assert.h>

/**
 *  Note: new items should be added to bottom, otherwise saved index is lost
 */
static const SimplePolar WinPilotPolars[] =
{
  { _T("206 Hornet"), 318, 100, 80, -0.606, 120, -0.99, 160, -1.918, 9.8},
  { _T("604 Kestrel"), 570, 100, 112.97, -0.72, 150.64, -1.42, 207.13, -4.1, 16.26},
  { _T("ASG-29 (15m)"), 362, 165, 108.8, -0.635, 156.4, -1.182, 211.13, -2.540, 9.20},
  { _T("ASG-29 (18m)"), 355, 225, 85, -0.47, 90, -0.48, 185, -2.00, 10.5},
  { _T("ASG-29E (18m)"), 400, 200, 90, -0.499, 95.5, -0.510, 196.4, -2.12, 10.5},
  { _T("ASH-25 (PAS)"), 693, 120, 105.67, -0.56, 163.25, -1.34, 211.26, -2.5, 16.31},
  { _T("ASH-25 (PIL)"), 602, 120, 98.5, -0.52, 152.18, -1.25, 196.93, -2.3, 16.31},
  { _T("ASH-25M (PAS)"), 750, 121, 130.01, -0.78, 169.96, -1.4, 219.94, -2.6, 16.31},
  { _T("ASH-25M (PIL)"), 660, 121, 121.3, -0.73, 159.35, -1.31, 206.22, -2.4, 16.31},
  { _T("ASH-26E"), 435, 90, 90, -0.51, 96, -0.53, 185, -2.00, 11.7},
  { _T("ASK-21 (PAS)"), 562, 0, 74.1, -0.75, 101.9, -0.87, 166.7, -2.44, 17.95},
  { _T("ASK-21 (PIL)"), 468, 0, 74.1, -0.67, 101.9, -0.90, 166.7, -2.68, 17.95},
  { _T("ASK-23"), 330, 0, 100, -0.85, 120, -1.19, 150, -2.02, 12.9},
  { _T("ASW-12"), 394, 189, 95, -0.57, 148, -1.48, 183.09, -2.6, 13.00},
  { _T("ASW-15"), 349, 91, 97.56, -0.77, 156.12, -1.9, 195.15, -3.4, 11.0},
  { _T("ASW-17"), 522, 151, 114.5, -0.7, 169.05, -1.68, 206.5, -2.9, 14.84},
  { _T("ASW-19"), 363, 125, 97.47, -0.74, 155.96, -1.64, 194.96, -3.1, 11.0},
  { _T("ASW-20"), 377, 159, 116.2, -0.77, 174.3, -1.89, 213.04, -3.3, 10.5},
  { _T("ASW-24"), 350, 159, 108.82, -0.73, 142.25, -1.21, 167.41, -1.8, 10.0},
  { _T("ASW-27"), 357, 165, 108.8, -0.64, 156.4, -1.18, 211.13, -2.5, 9.0},
  { _T("ASW-28 (18m)"), 345, 190, 65, -0.47, 107, -0.67, 165, -2.00, 10.5},
  { _T("DG-300"), 310, 190, 95.0, -0.66, 140.0, -1.28, 160.0, -1.70, 10.27},
  { _T("DG-400 (15m)"), 440, 90, 115, -0.76, 160.53, -1.22, 210.22, -2.3, 10.0},
  { _T("DG-400 (17m)"), 444, 90, 118.28, -0.68, 163.77, -1.15, 198.35, -1.8, 10.57},
  { _T("DG-500M (PAS)"), 750, 100, 121.6, -0.75, 162.12, -1.37, 202.66, -2.5, 18.29},
  { _T("DG-500M (PIL)"), 659, 100, 115.4, -0.71, 152.01, -1.28, 190.02, -2.3, 18.29},
  { _T("DG-500 (PAS)"), 660, 160, 115.5, -0.72, 152.16, -1.28, 190.22, -2.3, 18.29},
  { _T("DG-500 (PIL)"), 570, 160, 107.5, -0.66, 141.33, -1.19, 176.66, -2.1, 18.29},
  { _T("DG-800 15m"), 468, 120, 133.9, -0.88, 178.87, -1.53, 223.59, -2.5, 10.68},
  { _T("DG-800 18m"), 472, 120, 106, -0.62, 171.75, -1.47, 214.83, -2.4, 11.81},
  { _T("Discus"), 350, 182, 103.77, -0.72, 155.65, -1.55, 190.24, -3.1, 10.58},
  { _T("Duo Discus (PAS)"), 628, 201, 106.5, -0.79, 168.11, -1.54, 201.31, -2.9, 16.40},
  { _T("Duo Discus (PIL)"), 537, 201, 94.06, -0.72, 155.49, -1.43, 188.21, -2.7, 16.40},
  { _T("Duo Discus xT (PAS)"), 700, 50, 110, -0.664, 155, -1.206, 200, -2.287, 16.40},
  { _T("Duo Discus xT (PIL)"), 580, 170, 100, -0.605, 150, -1.271, 200, -2.668, 16.40},
  { _T("Genesis II"), 374, 151, 94, -0.61, 141.05, -1.18, 172.4, -2.0, 11.24},
  { _T("G 102 Astir CS"), 330, 90, 75.0, -0.7, 93.0, -0.74, 185.00, -3.1, 12.40},
  { _T("G 103 Twin II (PAS)"), 580, 0, 99, -0.8, 175.01, -1.95, 225.02, -3.8, 17.52},
  { _T("G 103 Twin II (PIL)"), 494, 0, 90.75, -0.74, 161.42, -1.8, 207.54, -3.5, 17.52},
  { _T("G 104 Speed Astir"), 351, 90, 90, -0.63, 105, -0.72, 157, -2.00, 11.5},
  { _T("H-201 Std Libelle"), 304, 50, 97, -0.79, 152.43, -1.91, 190.54, -3.3, 9.8},
  { _T("H-301 Libelle"), 300, 50, 94, -0.68, 147.71, -2.03, 184.64, -4.1, 9.8},
  { _T("IS-29D2 Lark"), 360, 0, 100, -0.82, 135.67, -1.55, 184.12, -3.3, 10.4},
  { _T("Janus B (PAS)"), 603, 170, 115.5, -0.76, 171.79, -1.98, 209.96, -4.0, 17.4},
  { _T("Janus B (PIL)"), 508, 170, 105.7, -0.7, 157.65, -1.82, 192.68, -3.6, 17.4},
  { _T("Ka 6CR"), 310, 0, 87.35, -0.81, 141.92, -2.03, 174.68, -3.5, 12.4},
  { _T("Ka 8"), 290, 0, 74.1, -0.76, 101.9, -1.27, 166.7, -4.64, 14.15},
  { _T("L 33 Solo"), 330, 0, 87.2, -0.8, 135.64, -1.73, 174.4, -3.4, 11.0},
  { _T("LAK17a (18m)"), 298, 180, 115, -0.680, 158, -1.379, 200, -2.975, 9.80},
  { _T("LAK17a (15m)"), 285, 180, 95, -0.574, 148, -1.310, 200, -2.885, 9.06},
  { _T("LS-1c"), 350, 91, 115.87, -1.02, 154.49, -1.84, 193.12, -3.3, 9.74},
  { _T("LS-3"), 383, 121, 93.0, -0.64, 127.0, -0.93, 148.2, -1.28, 10.5},
  { _T("LS-4a"), 361, 121, 114.9, -0.80, 172.3, -2.33, 210.59, -4.5, 10.35},
  { _T("LS-7wl"), 350, 150, 103.77, -0.73, 155.65, -1.47, 180.00, -2.66, 9.80},
  { _T("LS-6 (15m)"), 327, 160, 90, -0.6, 100, -0.658, 183, -1.965, 10.53},
  { _T("LS-6 (18m)"), 330, 140, 90, -0.51, 100, -0.57, 183, -2.00, 11.4},
  { _T("LS-8 (15m)"), 325, 185, 70, -0.51, 115, -0.85, 173, -2.00, 10.5},
  { _T("LS-8 (18m)"), 325, 185, 80, -0.51, 94, -0.56, 173, -2.00, 11.4},
  { _T("Nimbus 2"), 493, 159, 119.83, -0.75, 179.75, -2.14, 219.69, -3.8, 14.41},
  { _T("Nimbus 3DM (PAS)"), 820, 168, 114.97, -0.57, 157.42, -0.98, 222.24, -2.3, 16.70},
  { _T("Nimbus 3D (PAS)"), 712, 168, 93.64, -0.46, 175.42, -1.48, 218.69, -2.5, 16.70},
  { _T("Nimbus 3D (PIL)"), 621, 168, 87.47, -0.43, 163.86, -1.38, 204.27, -2.3, 16.70},
  { _T("Nimbus 3"), 527, 159, 116.18, -0.67, 174.28, -1.81, 232.37, -3.8, 16.70},
  { _T("Nimbus 3T"), 577, 310, 141.7, -0.99, 182.35, -1.89, 243.13, -4.0, 16.70},
  { _T("Nimbus 4DM (PAS)"), 820, 168, 100.01, -0.48, 150.01, -0.87, 190.76, -1.6, 17.8},
  { _T("Nimbus 4DM (PIL)"), 729, 168, 94.31, -0.46, 141.47, -0.82, 179.9, -1.5, 17.8},
  { _T("Nimbus 4D (PAS)"), 743, 303, 107.5, -0.5, 142.74, -0.83, 181.51, -1.6, 17.8},
  { _T("Nimbus 4D (PIL)"), 652, 303, 99, -0.46, 133.73, -0.78, 170.07, -1.5, 17.8},
  { _T("Nimbus 4"), 597, 303, 85.1, -0.41, 127.98, -0.75, 162.74, -1.4, 17.8},
  { _T("PIK-20B"), 354, 144, 102.5, -0.69, 157.76, -1.59, 216.91, -3.6, 10.0},
  { _T("PIK-20D"), 348, 144, 100, -0.69, 156.54, -1.78, 215.24, -4.2, 10.0},
  { _T("PIK-20E"), 437, 80, 109.61, -0.83, 166.68, -2, 241.15, -4.7, 10.0},
  { _T("PIK-30M"), 460, 0, 123.6, -0.78, 152.04, -1.12, 200.22, -2.2, 10.63},
  { _T("PW-5 Smyk"), 300, 0, 99.5, -0.95, 158.48, -2.85, 198.1, -5.1, 10.16},
  { _T("Russia AC-4"), 250, 0, 99.3, -0.92, 140.01, -1.8, 170.01, -2.9, 7.70},
  { _T("SGS 1-26E"), 315, 0, 82.3, -1.04, 117.73, -1.88, 156.86, -3.8, 14.87},
  { _T("SGS 1-34"), 354, 0, 89.82, -0.8, 143.71, -2.1, 179.64, -3.8, 14.03},
  { _T("SGS 1-35A"), 381, 179, 98.68, -0.74, 151.82, -1.8, 202.87, -3.9, 9.64},
  { _T("SGS 1-36 Sprite"), 322, 0, 75.98, -0.68, 132.96, -2, 170.95, -4.1, 13.10},
  { _T("Std Cirrus"), 337, 80, 93.23, -0.74, 149.17, -1.71, 205.1, -4.2, 10.04},
  { _T("Stemme S-10 (PAS)"), 850, 0, 133.47, -0.83, 167.75, -1.41, 205.03, -2.3, 18.70},
  { _T("Stemme S-10 (PIL)"), 759, 0, 125.8, -0.82, 158.51, -1.33, 193.74, -2.2, 18.70},
  { _T("SZD-36 Cobra"), 350, 30, 70.8, -0.60, 94.5, -0.69, 148.1, -1.83, 11.6},
  { _T("SZD-42A Jantar II"), 482, 191, 109.5, -0.66, 157.14, -1.47, 196.42, -2.7, 14.27},
  { _T("SZD-55-1 Promyk"), 350, 200, 100.0, -0.66, 120, -0.86, 150, -1.4, 9.60},
  { _T("Ventus a/b (16.6m)"), 358, 151, 100.17, -0.64, 159.69, -1.47, 239.54, -4.3, 9.96},
  { _T("Ventus b (15m)"), 341, 151, 97.69, -0.68, 156.3, -1.46, 234.45, -3.9, 9.51},
  { _T("Ventus cM"), 430, 0, 100.17, -0.6, 159.7, -1.32, 210.54, -2.5, 10.14},
  { _T("Ventus 2c (18m)"), 385, 180, 80.0, -0.5, 120.0, -0.73, 180.0, -2.0, 11.03},
  { _T("Ventus 2cx (18m)"), 385, 215, 80.0, -0.5, 120.0, -0.73, 180.0, -2.0, 11.03},
  { _T("Zuni II"), 358, 182, 110, -0.88, 167, -2.21, 203.72, -3.6, 10.13},

  { _T("Para EN C/DHV2"), 110, 4.19, 33.0, -1.0, 39.0, -1.2, 56.0, -2.30, 0},

  { _T("401 Kestrel 17m"), 367, 33, 95, -0.62, 110, -0.76, 175, -2.01, 11.58 },
  { _T("304CZ"), 310, 115, 115.03, -0.86, 174.04, -1.76, 212.72, -3.4, 0},
  { _T("IS-28B2 (PIL)"), 520, 0, 95, -0.78, 160, -2.47, 200, -5.2, 18.24},
  { _T("IS-28B2 (PAS)"), 590, 0, 100, -0.82, 160, -2.28, 200, -4.27, 18.24},
  { _T("SZD-30 Pirat"), 370, 0, 80, -0.72, 100, -0.98, 150, -2.46, 13.8},

  { _T("Para EN A/DHV1"), 100, 0, 29.0, -1.1,  34.0, -1.3, 44.0, -2.30, 0},
  { _T("Para EN B/DHV12"), 100, 0, 29.5, -1.1, 37.0, -1.2, 50.0, -2.30, 0},
  { _T("Para EN D/DHV23"), 100, 0, 33.0, -1.1, 41.0, -1.2, 58.0, -2.30,  0},
  { _T("Para Competition"), 100, 0, 39.0, -1.0, 45.0,  -1.1, 64.0, -2.30,  0},
  { _T("Delta USHPA-2"), 100, 0, 30, -1.10, 44.3,  -1.52,  58.0, -3.60,  0},
  { _T("Delta USHPA-3"), 100, 0, 37, -0.95, 48.1,  -1.15,  73.0, -3.60,  0},
  { _T("Delta USHPA-4"), 100, 0, 37, -0.89, 48.3,  -1.02,  76.5, -3.30,  0},

  { _T("Ventus 2cT (18m)"), 410, 110, 100.0, -0.62, 150.0, -1.2, 200.0, -2.3, 11.03},
  { _T("Ventus 2cxT (18m)"), 470, 130, 100.0, -0.56, 150.0, -1.13, 200.0, -2.28, 11.03},
  { _T("Blanik L13"), 472, 0, 85.0, -0.84, 143.0, -3.32, 200.0, -9.61, 19.1},
  { _T("Blanik L23"), 510, 0, 95.0, -0.94, 148.0, -2.60, 200.0, -6.37, 19.1},
  { _T("DG1000-20M (PIL)"), 490, 160, 100.0, -0.59, 120.0, -0.86, 150.0, -1.65, 17.51},
  { _T("DG1000-20M (PAS)"), 613, 160, 106.0, -0.62, 153.0, -1.53, 200.0, -3.2, 17.51},
  { _T("Mosquito"), 450, 0, 100.0, -0.68, 120.0, -0.92, 150.0, -1.45, 9.85},
  { _T("DuoDiscus T (PAS)"), 615, 80, 103, -0.64, 152, -1.25, 200, -2.51, 16.4},
  { _T("Jantar Std 48-2"), 375, 150, 100, -0.73, 120, -0.95, 150, -1.60, 10.66},
  { _T("Mini Nimbus"), 345, 155, 100, -0.69, 120, -0.92, 150, -1.45, 9.86},
  { _T("Pegase 101A"), 344, 120, 85.0, -0.62, 105, -0.75, 175, -2.54, 10.5},

  { _T("SZD-48-3 Jantar 3"), 326, 150, 95, -0.66, 180, -2.24, 220, -3.85, 10.66},
  { _T("SZD-51-1 Junior"), 333, 0, 70, -0.58, 130, -1.6, 180, -3.6, 12.51},
  { _T("SZD-9 bis 1E Bocian"), 540, 0, 70, -0.83, 90, -1.00, 140, -2.53, 20},

  { _T("ASW-22"), 597, 303, 80, -0.4015, 120, -0.66, 160, -1.3539, 16.31},
  { _T("Discus 2b"), 312, 200, 105, -0.66, 150, -1.05, 200, -2.0, 10.6},
  { _T("Ka 2b (PAS)"), 418, 0, 87, -0.9, 120, -1.5, 150, -2.6, 17.5},
  { _T("Ka 4 Rhoenlerche (PAS)"), 360, 0, 65, -0.95, 120, -2.5, 140, -3.5, 16.34},
  { _T("Ka 7 (PAS)"), 445, 0, 87, -0.92, 120, -1.55, 150, -2.7, 17.5},
  { _T("ASK-13 (PAS)"), 456, 0, 85, -0.84, 120, -1.5, 150, -2.8, 17.5},
};

/**
 * Returns the name of the internal WinPilot polar defined
 * by the array id i
 * @param i Array id of the polar
 * @return The name of the polar
 */
const TCHAR*
PolarStore::GetName(unsigned i)
{
  assert(i < Count());
  return WinPilotPolars[i].name;
}

/**
 * Reads internal WinPilot polar and passes it to
 * the converter
 * @param i Array id of the polar
 * @param polar Polar to set
 */
bool
PolarStore::Read(unsigned i, SimplePolar &polar)
{
  assert(i < Count());
  polar = WinPilotPolars[i];
  return true;
}

unsigned
PolarStore::Count()
{
  return sizeof(WinPilotPolars) / sizeof(WinPilotPolars[0]);
}
