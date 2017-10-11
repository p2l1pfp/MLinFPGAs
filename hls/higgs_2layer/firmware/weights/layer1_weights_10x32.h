weight_t weights1[N_INPUTS][N_LAYER_1] = 
  {{ 0.39704818,  0.11052962,  0.49147141,  0.04429482, -0.51528579,
     0.43868372,  0.07394741,  0.41149443,  0.40537292, -0.12150365,
     -0.29116538, -0.51228416, -0.00413968,  0.02055437, -0.0410219 ,
     0.39046121, -0.30009499, -0.29731396, -0.03945308,  0.27633348,
     0.36835453,  0.24040532, -0.39705363,  0.22418381,  0.28237239,
     0.29389915,  0.47315338, -0.37156004,  0.25447741,  0.25813472,
     0.47044075, -0.24790508},
   { 0.42456552,  0.50842226, -0.28124237,  0.07598961,  0.16559643,
     0.27212402, -0.02848247,  0.26143306,  0.26570681,  0.25181869,
     0.43451133,  0.11887478,  0.4564501 ,  0.02155816,  0.51329356,
     -0.37690166,  0.41744545, -0.48908111, -0.30502591, -0.14991593,
     0.02189334,  0.16674115,  0.46969512, -0.12109958, -0.48637429,
     0.04092315,  0.51478344, -0.34866416, -0.20418052, -0.54007727,
     -0.4592379 ,  0.14666054},
   {-0.2910355 ,  0.23629969, -0.19336547, -0.11605843, -0.48730484,
    0.42643118, -0.41360286,  0.25080752, -0.01924467,  0.01058839,
    -0.52074045, -0.43785965,  0.54119432, -0.38742343,  0.40023929,
    0.07841978,  0.3247484 , -0.01427439,  0.17446299,  0.17879274,
    0.31502408,  0.10919496, -0.16587155,  0.30072531, -0.41447324,
    0.46763372,  0.30262309,  0.05030759, -0.13267478,  0.40705645,
    -0.00146032, -0.36519453},
   {-0.27112383, -0.54888797,  0.01256107, -0.25181842,  0.02141548,
    0.50449824,  0.23051606, -0.07789572,  0.30554798,  0.4539896 ,
    -0.06285489, -0.0439182 ,  0.0639769 , -0.01103142,  0.41472441,
    0.38541579,  0.09512734, -0.23289892, -0.51073116, -0.48773351,
    -0.06764748, -0.50647515,  0.36689603, -0.3646743 , -0.46467206,
    0.5409894 , -0.08111433,  0.39485261, -0.04032158, -0.23896796,
    -0.00641616,  0.49067798},
   { 0.15660994,  0.13159873, -0.27551714,  0.38148221,  0.4074221 ,
     -0.37833661,  0.3574852 ,  0.25625548,  0.47962427, -0.28052756,
     0.2403999 , -0.42068362,  0.32458386,  0.02397615,  0.13542214,
     -0.41211683, -0.37346733,  0.24593049, -0.47551575,  0.13810907,
     0.21989772,  0.04789035, -0.36907804, -0.21096842,  0.171753  ,
     -0.38719964,  0.13517092, -0.39176571, -0.20627855,  0.19885483,
     0.50492471, -0.08417385},
   { 0.11383504, -0.52098936, -0.45827585,  0.35150507, -0.47178721,
     -0.02630239,  0.03807285, -0.26627573, -0.03279255,  0.26878658,
     0.36244345,  0.09105584, -0.05467638, -0.46407649, -0.29931879,
     -0.35314959,  0.41706231,  0.50721276, -0.02678821,  0.49776983,
     0.13284053, -0.00693853,  0.34539998,  0.41319111, -0.14087899,
     0.38161325,  0.21403316, -0.53948218, -0.00884281, -0.48860043,
     -0.49063122,  0.22909734},
   {-0.40476122,  0.32104293, -0.44226038, -0.23536749,  0.28394288,
    0.31516886,  0.31144696,  0.0706864 ,  0.39260045,  0.15438902,
    0.40563762,  0.25507483,  0.05392808,  0.027334  , -0.10675551,
    -0.26791999, -0.09579297, -0.23886752,  0.13536339,  0.42223743,
    -0.21617141, -0.00716528, -0.51513892,  0.10808346,  0.43726927,
    0.44901434, -0.20544149,  0.29413861,  0.36300579, -0.21726222,
    0.22776826, -0.46210265},
   { 0.26980248,  0.00846974,  0.53326404, -0.33950996,  0.10820107,
     -0.27779731, -0.45691302, -0.5081318 , -0.12712537, -0.07201028,
     0.32439932, -0.14907348, -0.15082592,  0.16869341, -0.29902494,
     0.24307188, -0.02083065,  0.31769559, -0.21182564,  0.4967972 ,
     -0.36991611,  0.35717604, -0.0967106 , -0.36323747,  0.37316561,
     -0.32743701,  0.27676573, -0.54012555,  0.15686898, -0.24642949,
     -0.18049742,  0.06380244},
   { 0.53111976,  0.00493514, -0.23663098, -0.2583037 , -0.06486508,
     -0.01755047,  0.06598504,  0.05648753, -0.10392567,  0.15993431,
     0.30490953, -0.10040963,  0.51808429, -0.31571773, -0.52640605,
     -0.16865306,  0.26318622,  0.06280956, -0.24569796,  0.04902239,
     0.44317079,  0.42563292, -0.11415254, -0.4172917 , -0.08495516,
     -0.52588886, -0.27332318, -0.39585212,  0.1151894 ,  0.24683471,
     -0.10189856, -0.49655285},
   {-0.44885519,  0.0327062 ,  0.09388136,  0.42111096,  0.10960019,
    0.18512489, -0.23649047, -0.47525775,  0.43734774, -0.1521987 ,
    -0.26001683, -0.53041565, -0.39290944,  0.04510847, -0.09442781,
    -0.16447625,  0.40120608,  0.3715364 ,  0.25422242, -0.15166654,
    0.25330067,  0.47681502,  0.21176694,  0.50897568, -0.30636644,
    0.49429014,  0.23605736, -0.15142258,  0.34467962, -0.14997362,
    0.3608681 ,  0.07667735}};
