/*
 *
 * Copyright 2021-2024 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#include "polar_code_impl.h"
#include "srsran/srsvec/copy.h"
#include "srsran/support/srsran_assert.h"

using namespace srsran;

const std::array<uint16_t, 32> srsran::polar_code_impl::mother_code_5 = {0,  1,  2,  4,  8,  16, 3,  5,  9,  6,  17,
                                                                         10, 18, 12, 20, 24, 7,  11, 19, 13, 14, 21,
                                                                         26, 25, 22, 28, 15, 23, 27, 29, 30, 31};

const std::array<uint16_t, 64> srsran::polar_code_impl::mother_code_6 = {
    0,  1,  2,  4,  8,  16, 32, 3,  5,  9,  6,  17, 10, 18, 12, 33, 20, 34, 24, 36, 7,  11,
    40, 19, 13, 48, 14, 21, 35, 26, 37, 25, 22, 38, 41, 28, 42, 49, 44, 50, 15, 52, 23, 56,
    27, 39, 29, 43, 30, 45, 51, 46, 53, 54, 57, 58, 60, 31, 47, 55, 59, 61, 62, 63};

const std::array<uint16_t, 128> srsran::polar_code_impl::mother_code_7 = {
    0,   1,  2,   4,   8,   16,  32,  3,   5,   64,  9,   6,   17,  10,  18,  12,  33,  65, 20,  34, 24,  36,
    7,   66, 11,  40,  68,  19,  13,  48,  14,  72,  21,  35,  26,  80,  37,  25,  22,  38, 96,  67, 41,  28,
    69,  42, 49,  74,  70,  44,  81,  50,  73,  15,  52,  23,  76,  82,  56,  27,  97,  39, 84,  29, 43,  98,
    88,  30, 71,  45,  100, 51,  46,  75,  104, 53,  77,  54,  83,  57,  112, 78,  85,  58, 99,  86, 60,  89,
    101, 31, 90,  102, 105, 92,  47,  106, 55,  113, 79,  108, 59,  114, 87,  116, 61,  91, 120, 62, 103, 93,
    107, 94, 109, 115, 110, 117, 118, 121, 122, 63,  124, 95,  111, 119, 123, 125, 126, 127};

const std::array<uint16_t, 256> srsran::polar_code_impl::mother_code_8 = {
    0,   1,   2,   4,   8,   16,  32,  3,   5,   64,  9,   6,   17,  10,  18,  128, 12,  33,  65,  20,  34,  24,
    36,  7,   129, 66,  11,  40,  68,  130, 19,  13,  48,  14,  72,  21,  132, 35,  26,  80,  37,  25,  22,  136,
    38,  96,  67,  41,  144, 28,  69,  42,  49,  74,  160, 192, 70,  44,  131, 81,  50,  73,  15,  133, 52,  23,
    134, 76,  137, 82,  56,  27,  97,  39,  84,  138, 145, 29,  43,  98,  88,  140, 30,  146, 71,  161, 45,  100,
    51,  148, 46,  75,  104, 162, 53,  193, 152, 77,  164, 54,  83,  57,  112, 135, 78,  194, 85,  58,  168, 139,
    99,  86,  60,  89,  196, 141, 101, 147, 176, 142, 31,  200, 90,  149, 102, 105, 163, 92,  47,  208, 150, 153,
    165, 106, 55,  113, 154, 79,  108, 224, 166, 195, 59,  169, 114, 156, 87,  197, 116, 170, 61,  177, 91,  198,
    172, 120, 201, 62,  143, 103, 178, 93,  202, 107, 180, 151, 209, 94,  204, 155, 210, 109, 184, 115, 167, 225,
    157, 110, 117, 212, 171, 226, 216, 158, 118, 173, 121, 199, 179, 228, 174, 122, 203, 63,  181, 232, 124, 205,
    182, 211, 185, 240, 206, 95,  213, 186, 227, 111, 214, 188, 217, 229, 159, 119, 218, 230, 233, 175, 123, 220,
    183, 234, 125, 241, 207, 187, 236, 126, 242, 244, 189, 215, 219, 231, 248, 190, 221, 235, 222, 237, 243, 238,
    245, 127, 191, 246, 249, 250, 252, 223, 239, 251, 247, 253, 254, 255};

const std::array<uint16_t, 512> srsran::polar_code_impl::mother_code_9 = {
    0,   1,   2,   4,   8,   16,  32,  3,   5,   64,  9,   6,   17,  10,  18,  128, 12,  33,  65,  20,  256, 34,  24,
    36,  7,   129, 66,  11,  40,  68,  130, 19,  13,  48,  14,  72,  257, 21,  132, 35,  258, 26,  80,  37,  25,  22,
    136, 260, 264, 38,  96,  67,  41,  144, 28,  69,  42,  49,  74,  272, 160, 288, 192, 70,  44,  131, 81,  50,  73,
    15,  320, 133, 52,  23,  134, 384, 76,  137, 82,  56,  27,  97,  39,  259, 84,  138, 145, 261, 29,  43,  98,  88,
    140, 30,  146, 71,  262, 265, 161, 45,  100, 51,  148, 46,  75,  266, 273, 104, 162, 53,  193, 152, 77,  164, 268,
    274, 54,  83,  57,  112, 135, 78,  289, 194, 85,  276, 58,  168, 139, 99,  86,  60,  280, 89,  290, 196, 141, 101,
    147, 176, 142, 321, 31,  200, 90,  292, 322, 263, 149, 102, 105, 304, 296, 163, 92,  47,  267, 385, 324, 208, 386,
    150, 153, 165, 106, 55,  328, 113, 154, 79,  269, 108, 224, 166, 195, 270, 275, 291, 59,  169, 114, 277, 156, 87,
    197, 116, 170, 61,  281, 278, 177, 293, 388, 91,  198, 172, 120, 201, 336, 62,  282, 143, 103, 178, 294, 93,  202,
    323, 392, 297, 107, 180, 151, 209, 284, 94,  204, 298, 400, 352, 325, 155, 210, 305, 300, 109, 184, 115, 167, 225,
    326, 306, 157, 329, 110, 117, 212, 171, 330, 226, 387, 308, 216, 416, 271, 279, 158, 337, 118, 332, 389, 173, 121,
    199, 179, 228, 338, 312, 390, 174, 393, 283, 122, 448, 353, 203, 63,  340, 394, 181, 295, 285, 232, 124, 205, 182,
    286, 299, 354, 211, 401, 185, 396, 344, 240, 206, 95,  327, 402, 356, 307, 301, 417, 213, 186, 404, 227, 418, 302,
    360, 111, 331, 214, 309, 188, 449, 217, 408, 229, 159, 420, 310, 333, 119, 339, 218, 368, 230, 391, 313, 450, 334,
    233, 175, 123, 341, 220, 314, 424, 395, 355, 287, 183, 234, 125, 342, 316, 241, 345, 452, 397, 403, 207, 432, 357,
    187, 236, 126, 242, 398, 346, 456, 358, 405, 303, 244, 189, 361, 215, 348, 419, 406, 464, 362, 409, 219, 311, 421,
    410, 231, 248, 369, 190, 364, 335, 480, 315, 221, 370, 422, 425, 451, 235, 412, 343, 372, 317, 222, 426, 453, 237,
    433, 347, 243, 454, 318, 376, 428, 238, 359, 457, 399, 434, 349, 245, 458, 363, 127, 191, 407, 436, 465, 246, 350,
    460, 249, 411, 365, 440, 374, 423, 466, 250, 371, 481, 413, 366, 468, 429, 252, 373, 482, 427, 414, 223, 472, 455,
    377, 435, 319, 484, 430, 488, 239, 378, 459, 437, 380, 461, 496, 351, 467, 438, 251, 462, 442, 441, 469, 247, 367,
    253, 375, 444, 470, 483, 415, 485, 473, 474, 254, 379, 431, 489, 486, 476, 439, 490, 463, 381, 497, 492, 443, 382,
    498, 445, 471, 500, 446, 475, 487, 504, 255, 477, 491, 478, 383, 493, 499, 502, 494, 501, 447, 505, 506, 479, 508,
    495, 503, 507, 509, 510, 511};

const std::array<uint16_t, 1024> srsran::polar_code_impl::mother_code_10 = {
    0,    1,    2,    4,   8,   16,   32,   3,    5,    64,   9,    6,   17,   10,   18,   128,  12,  33,   65,   20,
    256,  34,   24,   36,  7,   129,  66,   512,  11,   40,   68,   130, 19,   13,   48,   14,   72,  257,  21,   132,
    35,   258,  26,   513, 80,  37,   25,   22,   136,  260,  264,  38,  514,  96,   67,   41,   144, 28,   69,   42,
    516,  49,   74,   272, 160, 520,  288,  528,  192,  544,  70,   44,  131,  81,   50,   73,   15,  320,  133,  52,
    23,   134,  384,  76,  137, 82,   56,   27,   97,   39,   259,  84,  138,  145,  261,  29,   43,  98,   515,  88,
    140,  30,   146,  71,  262, 265,  161,  576,  45,   100,  640,  51,  148,  46,   75,   266,  273, 517,  104,  162,
    53,   193,  152,  77,  164, 768,  268,  274,  518,  54,   83,   57,  521,  112,  135,  78,   289, 194,  85,   276,
    522,  58,   168,  139, 99,  86,   60,   280,  89,   290,  529,  524, 196,  141,  101,  147,  176, 142,  530,  321,
    31,   200,  90,   545, 292, 322,  532,  263,  149,  102,  105,  304, 296,  163,  92,   47,   267, 385,  546,  324,
    208,  386,  150,  153, 165, 106,  55,   328,  536,  577,  548,  113, 154,  79,   269,  108,  578, 224,  166,  519,
    552,  195,  270,  641, 523, 275,  580,  291,  59,   169,  560,  114, 277,  156,  87,   197,  116, 170,  61,   531,
    525,  642,  281,  278, 526, 177,  293,  388,  91,   584,  769,  198, 172,  120,  201,  336,  62,  282,  143,  103,
    178,  294,  93,   644, 202, 592,  323,  392,  297,  770,  107,  180, 151,  209,  284,  648,  94,  204,  298,  400,
    608,  352,  325,  533, 155, 210,  305,  547,  300,  109,  184,  534, 537,  115,  167,  225,  326, 306,  772,  157,
    656,  329,  110,  117, 212, 171,  776,  330,  226,  549,  538,  387, 308,  216,  416,  271,  279, 158,  337,  550,
    672,  118,  332,  579, 540, 389,  173,  121,  553,  199,  784,  179, 228,  338,  312,  704,  390, 174,  554,  581,
    393,  283,  122,  448, 353, 561,  203,  63,   340,  394,  527,  582, 556,  181,  295,  285,  232, 124,  205,  182,
    643,  562,  286,  585, 299, 354,  211,  401,  185,  396,  344,  586, 645,  593,  535,  240,  206, 95,   327,  564,
    800,  402,  356,  307, 301, 417,  213,  568,  832,  588,  186,  646, 404,  227,  896,  594,  418, 302,  649,  771,
    360,  539,  111,  331, 214, 309,  188,  449,  217,  408,  609,  596, 551,  650,  229,  159,  420, 310,  541,  773,
    610,  657,  333,  119, 600, 339,  218,  368,  652,  230,  391,  313, 450,  542,  334,  233,  555, 774,  175,  123,
    658,  612,  341,  777, 220, 314,  424,  395,  673,  583,  355,  287, 183,  234,  125,  557,  660, 616,  342,  316,
    241,  778,  563,  345, 452, 397,  403,  207,  674,  558,  785,  432, 357,  187,  236,  664,  624, 587,  780,  705,
    126,  242,  565,  398, 346, 456,  358,  405,  303,  569,  244,  595, 189,  566,  676,  361,  706, 589,  215,  786,
    647,  348,  419,  406, 464, 680,  801,  362,  590,  409,  570,  788, 597,  572,  219,  311,  708, 598,  601,  651,
    421,  792,  802,  611, 602, 410,  231,  688,  653,  248,  369,  190, 364,  654,  659,  335,  480, 315,  221,  370,
    613,  422,  425,  451, 614, 543,  235,  412,  343,  372,  775,  317, 222,  426,  453,  237,  559, 833,  804,  712,
    834,  661,  808,  779, 617, 604,  433,  720,  816,  836,  347,  897, 243,  662,  454,  318,  675, 618,  898,  781,
    376,  428,  665,  736, 567, 840,  625,  238,  359,  457,  399,  787, 591,  678,  434,  677,  349, 245,  458,  666,
    620,  363,  127,  191, 782, 407,  436,  626,  571,  465,  681,  246, 707,  350,  599,  668,  790, 460,  249,  682,
    573,  411,  803,  789, 709, 365,  440,  628,  689,  374,  423,  466, 793,  250,  371,  481,  574, 413,  603,  366,
    468,  655,  900,  805, 615, 684,  710,  429,  794,  252,  373,  605, 848,  690,  713,  632,  482, 806,  427,  904,
    414,  223,  663,  692, 835, 619,  472,  455,  796,  809,  714,  721, 837,  716,  864,  810,  606, 912,  722,  696,
    377,  435,  817,  319, 621, 812,  484,  430,  838,  667,  488,  239, 378,  459,  622,  627,  437, 380,  818,  461,
    496,  669,  679,  724, 841, 629,  351,  467,  438,  737,  251,  462, 442,  441,  469,  247,  683, 842,  738,  899,
    670,  783,  849,  820, 728, 928,  791,  367,  901,  630,  685,  844, 633,  711,  253,  691,  824, 902,  686,  740,
    850,  375,  444,  470, 483, 415,  485,  905,  795,  473,  634,  744, 852,  960,  865,  693,  797, 906,  715,  807,
    474,  636,  694,  254, 717, 575,  913,  798,  811,  379,  697,  431, 607,  489,  866,  723,  486, 908,  718,  813,
    476,  856,  839,  725, 698, 914,  752,  868,  819,  814,  439,  929, 490,  623,  671,  739,  916, 463,  843,  381,
    497,  930,  821,  726, 961, 872,  492,  631,  729,  700,  443,  741, 845,  920,  382,  822,  851, 730,  498,  880,
    742,  445,  471,  635, 932, 687,  903,  825,  500,  846,  745,  826, 732,  446,  962,  936,  475, 853,  867,  637,
    907,  487,  695,  746, 828, 753,  854,  857,  504,  799,  255,  964, 909,  719,  477,  915,  638, 748,  944,  869,
    491,  699,  754,  858, 478, 968,  383,  910,  815,  976,  870,  917, 727,  493,  873,  701,  931, 756,  860,  499,
    731,  823,  922,  874, 918, 502,  933,  743,  760,  881,  494,  702, 921,  501,  876,  847,  992, 447,  733,  827,
    934,  882,  937,  963, 747, 505,  855,  924,  734,  829,  965,  938, 884,  506,  749,  945,  966, 755,  859,  940,
    830,  911,  871,  639, 888, 479,  946,  750,  969,  508,  861,  757, 970,  919,  875,  862,  758, 948,  977,  923,
    972,  761,  877,  952, 495, 703,  935,  978,  883,  762,  503,  925, 878,  735,  993,  885,  939, 994,  980,  926,
    764,  941,  967,  886, 831, 947,  507,  889,  984,  751,  942,  996, 971,  890,  509,  949,  973, 1000, 892,  950,
    863,  759,  1008, 510, 979, 953,  763,  974,  954,  879,  981,  982, 927,  995,  765,  956,  887, 985,  997,  986,
    943,  891,  998,  766, 511, 988,  1001, 951,  1002, 893,  975,  894, 1009, 955,  1004, 1010, 957, 983,  958,  987,
    1012, 999,  1016, 767, 989, 1003, 990,  1005, 959,  1011, 1013, 895, 1006, 1014, 1017, 1018, 991, 1020, 1007, 1015,
    1019, 1021, 1022, 1023};

const std::array<uint16_t, 32> srsran::polar_code_impl::blk_interleaver_5 = {0,  1,  2,  4,  3,  5,  6,  7,  8,  16, 9,
                                                                             17, 10, 18, 11, 19, 12, 20, 13, 21, 14, 22,
                                                                             15, 23, 24, 25, 26, 28, 27, 29, 30, 31};

const std::array<uint16_t, 64> srsran::polar_code_impl::blk_interleaver_6 = {
    0,  1,  2,  3,  4,  5,  8,  9,  6,  7,  10, 11, 12, 13, 14, 15, 16, 17, 32, 33, 18, 19,
    34, 35, 20, 21, 36, 37, 22, 23, 38, 39, 24, 25, 40, 41, 26, 27, 42, 43, 28, 29, 44, 45,
    30, 31, 46, 47, 48, 49, 50, 51, 52, 53, 56, 57, 54, 55, 58, 59, 60, 61, 62, 63};

const std::array<uint16_t, 128> srsran::polar_code_impl::blk_interleaver_7 = {
    0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  16,  17,  18,  19,  12,  13,  14,  15,  20,  21,
    22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  64,  65,  66,  67,  36,  37,  38,  39,
    68,  69,  70,  71,  40,  41,  42,  43,  72,  73,  74,  75,  44,  45,  46,  47,  76,  77,  78,  79,  48,  49,
    50,  51,  80,  81,  82,  83,  52,  53,  54,  55,  84,  85,  86,  87,  56,  57,  58,  59,  88,  89,  90,  91,
    60,  61,  62,  63,  92,  93,  94,  95,  96,  97,  98,  99,  100, 101, 102, 103, 104, 105, 106, 107, 112, 113,
    114, 115, 108, 109, 110, 111, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127};

const std::array<uint16_t, 256> srsran::polar_code_impl::blk_interleaver_8 = {
    0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,
    22,  23,  32,  33,  34,  35,  36,  37,  38,  39,  24,  25,  26,  27,  28,  29,  30,  31,  40,  41,  42,  43,
    44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,
    66,  67,  68,  69,  70,  71,  128, 129, 130, 131, 132, 133, 134, 135, 72,  73,  74,  75,  76,  77,  78,  79,
    136, 137, 138, 139, 140, 141, 142, 143, 80,  81,  82,  83,  84,  85,  86,  87,  144, 145, 146, 147, 148, 149,
    150, 151, 88,  89,  90,  91,  92,  93,  94,  95,  152, 153, 154, 155, 156, 157, 158, 159, 96,  97,  98,  99,
    100, 101, 102, 103, 160, 161, 162, 163, 164, 165, 166, 167, 104, 105, 106, 107, 108, 109, 110, 111, 168, 169,
    170, 171, 172, 173, 174, 175, 112, 113, 114, 115, 116, 117, 118, 119, 176, 177, 178, 179, 180, 181, 182, 183,
    120, 121, 122, 123, 124, 125, 126, 127, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197,
    198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 224, 225, 226, 227,
    228, 229, 230, 231, 216, 217, 218, 219, 220, 221, 222, 223, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241,
    242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};

const std::array<uint16_t, 512> srsran::polar_code_impl::blk_interleaver_9 = {
    0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,
    23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,
    46,  47,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  48,  49,  50,  51,  52,
    53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,
    92,  93,  94,  95,  96,  97,  98,  99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114,
    115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137,
    138, 139, 140, 141, 142, 143, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 144,
    145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 272, 273, 274, 275, 276, 277, 278, 279,
    280, 281, 282, 283, 284, 285, 286, 287, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
    175, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 176, 177, 178, 179, 180, 181,
    182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316,
    317, 318, 319, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 320, 321, 322, 323,
    324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218,
    219, 220, 221, 222, 223, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 224, 225,
    226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 352, 353, 354, 355, 356, 357, 358, 359, 360,
    361, 362, 363, 364, 365, 366, 367, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255,
    368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 382, 383, 384, 385, 386, 387, 388, 389, 390,
    391, 392, 393, 394, 395, 396, 397, 398, 399, 400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413,
    414, 415, 416, 417, 418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 448, 449, 450, 451, 452,
    453, 454, 455, 456, 457, 458, 459, 460, 461, 462, 463, 432, 433, 434, 435, 436, 437, 438, 439, 440, 441, 442, 443,
    444, 445, 446, 447, 464, 465, 466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477, 478, 479, 480, 481, 482,
    483, 484, 485, 486, 487, 488, 489, 490, 491, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505,
    506, 507, 508, 509, 510, 511};

const std::array<uint16_t, 1024> srsran::polar_code_impl::blk_interleaver_10 = {
    0,    1,    2,    3,    4,    5,    6,    7,    8,    9,    10,   11,   12,   13,   14,   15,   16,   17,   18,
    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   36,   37,
    38,   39,   40,   41,   42,   43,   44,   45,   46,   47,   48,   49,   50,   51,   52,   53,   54,   55,   56,
    57,   58,   59,   60,   61,   62,   63,   64,   65,   66,   67,   68,   69,   70,   71,   72,   73,   74,   75,
    76,   77,   78,   79,   80,   81,   82,   83,   84,   85,   86,   87,   88,   89,   90,   91,   92,   93,   94,
    95,   128,  129,  130,  131,  132,  133,  134,  135,  136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
    146,  147,  148,  149,  150,  151,  152,  153,  154,  155,  156,  157,  158,  159,  96,   97,   98,   99,   100,
    101,  102,  103,  104,  105,  106,  107,  108,  109,  110,  111,  112,  113,  114,  115,  116,  117,  118,  119,
    120,  121,  122,  123,  124,  125,  126,  127,  160,  161,  162,  163,  164,  165,  166,  167,  168,  169,  170,
    171,  172,  173,  174,  175,  176,  177,  178,  179,  180,  181,  182,  183,  184,  185,  186,  187,  188,  189,
    190,  191,  192,  193,  194,  195,  196,  197,  198,  199,  200,  201,  202,  203,  204,  205,  206,  207,  208,
    209,  210,  211,  212,  213,  214,  215,  216,  217,  218,  219,  220,  221,  222,  223,  224,  225,  226,  227,
    228,  229,  230,  231,  232,  233,  234,  235,  236,  237,  238,  239,  240,  241,  242,  243,  244,  245,  246,
    247,  248,  249,  250,  251,  252,  253,  254,  255,  256,  257,  258,  259,  260,  261,  262,  263,  264,  265,
    266,  267,  268,  269,  270,  271,  272,  273,  274,  275,  276,  277,  278,  279,  280,  281,  282,  283,  284,
    285,  286,  287,  512,  513,  514,  515,  516,  517,  518,  519,  520,  521,  522,  523,  524,  525,  526,  527,
    528,  529,  530,  531,  532,  533,  534,  535,  536,  537,  538,  539,  540,  541,  542,  543,  288,  289,  290,
    291,  292,  293,  294,  295,  296,  297,  298,  299,  300,  301,  302,  303,  304,  305,  306,  307,  308,  309,
    310,  311,  312,  313,  314,  315,  316,  317,  318,  319,  544,  545,  546,  547,  548,  549,  550,  551,  552,
    553,  554,  555,  556,  557,  558,  559,  560,  561,  562,  563,  564,  565,  566,  567,  568,  569,  570,  571,
    572,  573,  574,  575,  320,  321,  322,  323,  324,  325,  326,  327,  328,  329,  330,  331,  332,  333,  334,
    335,  336,  337,  338,  339,  340,  341,  342,  343,  344,  345,  346,  347,  348,  349,  350,  351,  576,  577,
    578,  579,  580,  581,  582,  583,  584,  585,  586,  587,  588,  589,  590,  591,  592,  593,  594,  595,  596,
    597,  598,  599,  600,  601,  602,  603,  604,  605,  606,  607,  352,  353,  354,  355,  356,  357,  358,  359,
    360,  361,  362,  363,  364,  365,  366,  367,  368,  369,  370,  371,  372,  373,  374,  375,  376,  377,  378,
    379,  380,  381,  382,  383,  608,  609,  610,  611,  612,  613,  614,  615,  616,  617,  618,  619,  620,  621,
    622,  623,  624,  625,  626,  627,  628,  629,  630,  631,  632,  633,  634,  635,  636,  637,  638,  639,  384,
    385,  386,  387,  388,  389,  390,  391,  392,  393,  394,  395,  396,  397,  398,  399,  400,  401,  402,  403,
    404,  405,  406,  407,  408,  409,  410,  411,  412,  413,  414,  415,  640,  641,  642,  643,  644,  645,  646,
    647,  648,  649,  650,  651,  652,  653,  654,  655,  656,  657,  658,  659,  660,  661,  662,  663,  664,  665,
    666,  667,  668,  669,  670,  671,  416,  417,  418,  419,  420,  421,  422,  423,  424,  425,  426,  427,  428,
    429,  430,  431,  432,  433,  434,  435,  436,  437,  438,  439,  440,  441,  442,  443,  444,  445,  446,  447,
    672,  673,  674,  675,  676,  677,  678,  679,  680,  681,  682,  683,  684,  685,  686,  687,  688,  689,  690,
    691,  692,  693,  694,  695,  696,  697,  698,  699,  700,  701,  702,  703,  448,  449,  450,  451,  452,  453,
    454,  455,  456,  457,  458,  459,  460,  461,  462,  463,  464,  465,  466,  467,  468,  469,  470,  471,  472,
    473,  474,  475,  476,  477,  478,  479,  704,  705,  706,  707,  708,  709,  710,  711,  712,  713,  714,  715,
    716,  717,  718,  719,  720,  721,  722,  723,  724,  725,  726,  727,  728,  729,  730,  731,  732,  733,  734,
    735,  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,  491,  492,  493,  494,  495,  496,  497,
    498,  499,  500,  501,  502,  503,  504,  505,  506,  507,  508,  509,  510,  511,  736,  737,  738,  739,  740,
    741,  742,  743,  744,  745,  746,  747,  748,  749,  750,  751,  752,  753,  754,  755,  756,  757,  758,  759,
    760,  761,  762,  763,  764,  765,  766,  767,  768,  769,  770,  771,  772,  773,  774,  775,  776,  777,  778,
    779,  780,  781,  782,  783,  784,  785,  786,  787,  788,  789,  790,  791,  792,  793,  794,  795,  796,  797,
    798,  799,  800,  801,  802,  803,  804,  805,  806,  807,  808,  809,  810,  811,  812,  813,  814,  815,  816,
    817,  818,  819,  820,  821,  822,  823,  824,  825,  826,  827,  828,  829,  830,  831,  832,  833,  834,  835,
    836,  837,  838,  839,  840,  841,  842,  843,  844,  845,  846,  847,  848,  849,  850,  851,  852,  853,  854,
    855,  856,  857,  858,  859,  860,  861,  862,  863,  896,  897,  898,  899,  900,  901,  902,  903,  904,  905,
    906,  907,  908,  909,  910,  911,  912,  913,  914,  915,  916,  917,  918,  919,  920,  921,  922,  923,  924,
    925,  926,  927,  864,  865,  866,  867,  868,  869,  870,  871,  872,  873,  874,  875,  876,  877,  878,  879,
    880,  881,  882,  883,  884,  885,  886,  887,  888,  889,  890,  891,  892,  893,  894,  895,  928,  929,  930,
    931,  932,  933,  934,  935,  936,  937,  938,  939,  940,  941,  942,  943,  944,  945,  946,  947,  948,  949,
    950,  951,  952,  953,  954,  955,  956,  957,  958,  959,  960,  961,  962,  963,  964,  965,  966,  967,  968,
    969,  970,  971,  972,  973,  974,  975,  976,  977,  978,  979,  980,  981,  982,  983,  984,  985,  986,  987,
    988,  989,  990,  991,  992,  993,  994,  995,  996,  997,  998,  999,  1000, 1001, 1002, 1003, 1004, 1005, 1006,
    1007, 1008, 1009, 1010, 1011, 1012, 1013, 1014, 1015, 1016, 1017, 1018, 1019, 1020, 1021, 1022, 1023};

span<const uint16_t> srsran::polar_code_impl::get_mother_code(uint8_t n_)
{
  switch (n_) {
    case 5:
      return mother_code_5;
    case 6:
      return mother_code_6;
    case 7:
      return mother_code_7;
    case 8:
      return mother_code_8;
    case 9:
      return mother_code_9;
    case 10:
      return mother_code_10;
    default:
      srsran_assertion_failure("Invalid n={}", n_);
  }
  // Unreachable
  return {};
}

span<const uint16_t> srsran::polar_code_impl::get_blk_interleaver(uint8_t n_)
{
  switch (n_) {
    case 5:
      return blk_interleaver_5;
    case 6:
      return blk_interleaver_6;
    case 7:
      return blk_interleaver_7;
    case 8:
      return blk_interleaver_8;
    case 9:
      return blk_interleaver_9;
    case 10:
      return blk_interleaver_10;
    default:
      srsran_assertion_failure("Invalid n={}", n_);
  }
  // Unreachable
  return {};
}

unsigned srsran::polar_code_impl::setdiff_stable(const uint16_t* x,
                                                 const uint16_t* y,
                                                 uint16_t*       z,
                                                 uint16_t        T,
                                                 uint16_t        len1,
                                                 uint16_t        len2)
{
  uint16_t o = 0;
  for (uint16_t i = 0; i != len1; ++i) {
    // is x[i] in y ?
    unsigned flag = 0;
    if (x[i] <= T) {
      flag = 1;
    } else {
      for (uint16_t j = 0; j != len2; ++j) {
        if (x[i] == y[j]) {
          flag = 1;
          break;
        }
      }
    }

    if (flag == 0) {
      z[o] = x[i];
      o++;
    }
  }
  return o;
}

void srsran::polar_code_impl::set_code_params(unsigned K_, unsigned E_, uint8_t nMax)
{
  // Set internal K
  K = K_;

  // Set internal E
  E = E_;

  // include here also npc and nwmPC computatoins
  srsran_assert(E <= EMAX, "Invalid E value '{}'", E);
  switch (nMax) {
    case 9: // downlink
      // iil = true
      // Codeblock length (K=%d) not supported for downlink transmission, choose 165 > K > 35
      srsran_assert(!(K < 36 || K > 164), "Invalid K range");
      break;
    case 10:
      // iil = false
      // Codeblock length (K=%d) not supported for uplink transmission, choose K > 17 and K < 1024, excluding 31 > K >
      // 25
      srsran_assert(!(K < 18 || (K > 25 && K < 31) || K > 1023), "Invalid K range");
      break;
    default:
      // nMax not supported choose 9 for downlink  and 10 for uplink transmissions
      srsran_assert(false, "Unreachable code");
  }

  // number of parity check bits (nPC) and parity check bits of minimum bandwidth nWmPC
  nPC   = 0;
  nWmPC = 0;
  if (K <= 25) {
    nPC = 3;
    if (E > K + 189) {
      nWmPC = 1;
    }
  }

  // Rate-matched codeword length (E) not supported, choose E > K + nPC
  srsran_assert(K + nPC < E, "Invalid K + nPC values");

  // determination of the codeword size (N)
  // ceil(log2(E))
  uint8_t e = 1;
  for (; e <= eMAX; ++e) {
    unsigned tmpE = 1U << e; // 2^e
    if (tmpE >= E) {
      break;
    }
  }

  uint8_t n1  = 0;
  uint8_t e_1 = e - 1;
  if ((8 * E <= 9 * (1U << e_1)) && (16 * K < 9 * E)) {
    n1 = e - 1;
  } else {
    n1 = e;
  }

  // ceil(log2(K))
  uint8_t k = 0;
  for (; k <= 10; ++k) {
    unsigned tmpK = 1U << k; // 2^e
    if (tmpK >= K) {
      break;
    }
  }
  uint8_t n2 = k + 3;

  // min(n1, n2, n3)
  n = n1;
  if (n2 < n1) {
    n = n2;
  }
  if (nMax < n) {
    n = nMax;
  }
  if (n < 5) {
    n = 5;
  }
  N = (1U << n);

  // Codeblock length (K) not supported, choose K < N
  srsran_assert(K < N, "Invalid K value");
}

srsran::polar_code_impl::polar_code_impl()
{
  // Do nothing
}

// Compares two unsigned
bool cmpfunc(unsigned ai, unsigned bi)
{
  return (ai < bi);
}

void srsran::polar_code_impl::set(unsigned K_, unsigned E_, uint8_t nMax, polar_code_ibil ibil_)
{
  // Check polar code parameters.
  set_code_params(K_, E_, nMax);

  span<const uint16_t> blk_interleaver = get_blk_interleaver(n);
  span<const uint16_t> mother_code     = get_mother_code(n);

  ibil = ibil_;

  // Select only the most reliable (message and parity).
  span<const uint16_t> K_set = mother_code.last(K + nPC);
  if (N > E) {
    // Frozen bits due to Puncturing and Shortening.
    unsigned       T              = 0;
    unsigned       tmp_F_set_size = N - E;
    unsigned       N_th           = 3 * N / 4;
    span<uint16_t> F_set          = span<uint16_t>(tmp_F_set);

    if (16 * K <= 7 * E) { // Puncturing
      if (E >= N_th) {
        T = N_th - (E >> 1U) - 1;
      } else {
        T = 9 * N / 16 - (E >> 2U);
      }
      srsvec::copy(F_set.first(tmp_F_set_size),
                   blk_interleaver.first(tmp_F_set_size)); // The first (less reliable) after interleaving

    } else { // Shortening
      srsvec::copy(F_set.first(tmp_F_set_size),
                   blk_interleaver.subspan(E, tmp_F_set_size)); // The first (less reliable) after interleaving
    }

    unsigned tmp_K = setdiff_stable(mother_code.data(), F_set.data(), tmp_K_set.data(), T, N, tmp_F_set_size);
    K_set          = span<const uint16_t>(tmp_K_set).subspan(tmp_K - K - nPC, K + nPC);
  }

  // Take the nPC - nWmPC less reliable.
  for (unsigned i = 0, i_end = (nPC > nWmPC) ? (nPC - nWmPC) : 0; i != i_end; ++i) {
    PC_set[i] = K_set[i];
  }

  // This only happens if K=18:25 and E=E+189+1:8192.
  // In these cases, there is no puncturing or shortening.
  if (nWmPC == 1) {
    if (K <= 21) {
      PC_set[nPC - 1] = 252;
    } else {
      PC_set[nPC - 1] = 248;
    }
  }

  // Sort K_set (includes parity bits) using a mask.
  K_set_mask.resize(N);
  K_set_mask.fill(0, N, false);

  // Write the K_set in a mask.
  std::for_each(K_set.begin(), K_set.end(), [this](unsigned index) { K_set_mask.set(index); });

  // Create the frozen set F_set as the complement of sorted K_set.
  F_set_mask = ~K_set_mask;

  // Sorted PC_set.
  if (nPC > 0) {
    std::sort(PC_set.begin(), PC_set.begin() + nPC, cmpfunc);
  }

  // Mark the end of the sets (useful at subchannel allocation).
  PC_set[nPC] = NMAX;
}

unsigned polar_code_impl::get_n() const
{
  return n;
}

unsigned polar_code_impl::get_N() const
{
  return N;
}

unsigned polar_code_impl::get_K() const
{
  return K;
}

unsigned polar_code_impl::get_E() const
{
  return E;
}

unsigned polar_code_impl::get_nPC() const
{
  return nPC;
}

const bounded_bitset<polar_code::NMAX>& polar_code_impl::get_K_set() const
{
  return K_set_mask;
}

span<const uint16_t> polar_code_impl::get_PC_set() const
{
  return PC_set;
}

const bounded_bitset<polar_code::NMAX>& polar_code_impl::get_F_set() const
{
  return F_set_mask;
}

span<const uint16_t> polar_code_impl::get_blk_interleaver() const
{
  return get_blk_interleaver(n);
}

polar_code_ibil polar_code_impl::get_ibil() const
{
  return ibil;
}
