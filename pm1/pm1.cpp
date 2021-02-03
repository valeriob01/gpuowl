#include <cmath>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <cstdio>

#include <tuple>
#include <utility>
#include <string>
#include <vector>
#include <array>

using namespace std;
using u32 = uint32_t;
using u64 = uint64_t;

// Table of values of Dickman's "rho" function for argument from 2 in steps of 1/40.
// Was generated using dickman_rho() in SageMath (see https://sagecell.sagemath.org/ or https://cocalc.com/ )
double rhotab[] = {
0.306852819440055, 0.294583985324559, 0.282765004395792, 0.271377929030937, 0.260405780162154, 0.249832479607378, 0.239642788276221, 0.229822249637283, 0.220357137908328, 0.211234410495339, 0.202441664262192, 0.193967095260853, 0.185799461593866, 0.177928049117287, 0.170342639724018, 0.163033481975292, 0.155991263872504, 0.149207087583014, 0.142672445952511, 0.136379200653245, 0.130319561832251, 0.124486069136846, 0.118871574006370, 0.113469223129561, 0.108272442976271, 0.103274925320531, 0.0984706136794386, 0.0938536905990306, 0.0894185657243129, 0.0851598645960335, 0.0810724181216677, 0.0771512526724908, 0.0733915807625995, 0.0697887922693543, 0.0663384461579859, 0.0630362626760817, 0.0598781159863707, 0.0568600272086837, 0.0539781578442059, 0.0512288035571861, 
0.0486083882911316, 0.0461130326881283, 0.0437373330511146, 0.0414757372875022, 0.0393229695406371, 0.0372740138102934, 0.0353240987411619, 0.0334686834799628, 0.0317034445117801, 0.0300242633950183, 0.0284272153221808, 0.0269085584405901, 0.0254647238733285, 0.0240923063861712, 0.0227880556511908, 0.0215488680621084, 0.0203717790604077, 0.0192539559347745, 0.0181926910596145, 0.0171853955412799, 0.0162295932432360, 0.0153229151637531, 0.0144630941418387, 0.0136479598690614, 0.0128754341866765, 0.0121435266490617, 0.0114503303359322, 0.0107940178971306, 0.0101728378150057, 0.00958511087050379, 0.00902922680011186, 0.00850364113172792, 0.00800687218838523, 0.00753749824954556, 0.00709415486039758, 0.00667553228026170, 0.00628037306181464, 0.00590746975341183, 0.00555566271730628, 0.00522383805704407, 
0.00491092564776083, 0.00461589792831590, 0.00433777522517762, 0.00407563033516079, 0.00382858617381395, 0.00359581292778975, 0.00337652538864193, 0.00316998045479902, 0.00297547478958152, 0.00279234262413789, 0.00261995369508530, 0.00245771130746780, 0.00230505051439257, 0.00216143640538637, 0.00202636249613307, 0.00189934921281744, 0.00177994246481535, 0.00166771229994066, 0.00156225163688919, 0.00146317506991413, 0.00137011774112811, 0.00128273427615882, 0.00120069777918906, 0.00112369888369229, 0.00105144485543239, 0.000983658744533696, 0.000920078583646128, 0.000860456629432652, 0.000804558644792605, 0.000752163219407233, 0.000703061126353299, 0.000657054712678595, 0.000613957321970095, 0.000573592747072627, 0.000535794711233811, 0.000500406376060490, 0.000467279874773688, 0.000436275869343690, 0.000407263130174890, 0.000380118137092072, 
0.000354724700456040, 0.000330973600643576, 0.000308762228684552, 0.000287994203970583, 0.000268578998820779, 0.000250431585949591, 0.000233472107922766, 0.000217625567116417, 0.000202821534805516, 0.000188993878109908, 0.000176080503619378, 0.000164023116605003, 0.000152766994802780, 0.000142260775827954, 0.000132456257345164, 0.000123308209180912, 0.000114774196621564, 0.000106814414192339, 0.0000993915292610416, 0.0000924705348549445, 0.0000860186111205116, 0.0000800049948938775, 0.0000744008568854185, 0.0000691791860145715, 0.0000643146804615109, 0.0000597836450305498, 0.0000555638944463892, 0.0000516346622287225, 0.0000479765148133912, 0.0000445712706093859, 0.0000414019237006278, 0.0000384525719197640, 0.0000357083490382522, 0.0000331553608329049, 0.0000307806248038908, 0.0000285720133330149, 0.0000265182000840266, 0.0000246086094587567, 0.0000228333689341654, 0.0000211832641159171, 
0.0000196496963539553, 0.0000182246427752333, 0.0000169006186225834, 0.0000156706419007730, 0.0000145282003166539, 0.0000134672203844929, 0.0000124820385512393, 0.0000115673742064098, 0.0000107183044508680, 9.93024050764319e-6, 9.19890566611241e-6, 8.52031465844451e-6, 7.89075437420041e-6, 7.30676582546609e-6, 6.76512728089460e-6, 6.26283849259350e-6, 5.79710594495074e-6, 5.36532905927350e-6, 4.96508729255373e-6, 4.59412807279594e-6, 4.25035551717139e-6, 3.93181988282247e-6, 3.63670770345000e-6, 3.36333256789496e-6, 3.11012649979137e-6, 2.87563190003439e-6, 2.65849401629250e-6, 2.45745390610736e-6, 2.27134186228307e-6, 2.09907127127830e-6, 1.93963287719169e-6, 1.79208942568205e-6, 1.65557066379923e-6, 1.52926867322779e-6, 1.41243351587104e-6, 1.30436917203406e-6, 1.20442975270958e-6, 1.11201596863302e-6, 1.02657183986121e-6, 9.47581630646477e-7, 
8.74566995329392e-7, 8.07084321863891e-7, 7.44722260394541e-7, 6.87099424822591e-7, 6.33862255545582e-7, 5.84683032116228e-7, 5.39258025342825e-7, 4.97305779099376e-7, 4.58565512804405e-7, 4.22795636167001e-7, 3.89772368391109e-7, 3.59288454578188e-7, 3.31151972577348e-7, 3.05185224004181e-7, 2.81223703587451e-7, 2.59115141409215e-7, 2.38718612981323e-7, 2.19903712451561e-7, 2.02549784558224e-7, 1.86545211254284e-7, 1.71786749203399e-7, 1.58178914611321e-7, 1.45633412099219e-7, 1.34068604551440e-7, 1.23409021080502e-7, 1.13584900447731e-7, 1.04531767460094e-7, 9.61900400332510e-8, 8.85046647687321e-8, 8.14247790401044e-8, 7.49033977199179e-8, 6.88971228067640e-8, 6.33658743306062e-8, 5.82726410252914e-8, 5.35832493603539e-8, 4.92661496204184e-8, 4.52922178102003e-8, 4.16345722467027e-8, 3.82684037781748e-8, 3.51708186420984e-8, 
3.23206930422610e-8, 2.96985385882167e-8, 2.72863777994286e-8, 2.50676289318058e-8, 2.30269994373198e-8, 2.11503874179167e-8, 1.94247904820595e-8, 1.78382214558657e-8, 1.63796304411581e-8, 1.50388327501072e-8, 1.38064422807221e-8, 1.26738099294607e-8, 1.16329666668818e-8, 1.06765709297201e-8, 9.79786000820215e-9, 8.99060513099409e-9, 8.24906997200364e-9, 7.56797232350324e-9, 6.94244869879648e-9, 6.36802164503788e-9, 5.84056956293623e-9, 5.35629884501421e-9, 4.91171815795476e-9, 4.50361470741174e-9, 4.12903233557698e-9, 3.78525131284464e-9, 3.46976969515950e-9, 3.18028612813905e-9, 2.91468398787199e-9, 2.67101675646914e-9, 2.44749453802384e-9, 2.24247162766827e-9, 2.05443505293307e-9, 1.88199401266379e-9, 1.72387014435469e-9, 1.57888855596052e-9, 1.44596956306737e-9, 1.32412107677439e-9, 1.21243159178189e-9, 1.11006372802312e-9, 
1.01624828273784e-9, 9.30278753185683e-10, 8.51506293255724e-10, 7.79335070061403e-10, 7.13217989231916e-10, 6.52652760033187e-10, 5.97178273686798e-10, 5.46371270318092e-10, 4.99843271868294e-10, 4.57237760062391e-10, 4.18227580146182e-10, 3.82512552602544e-10, 3.49817276438660e-10, 3.19889108911136e-10, 2.92496307733140e-10, 2.67426322894375e-10, 2.44484226227652e-10, 2.23491267781831e-10, 2.04283548915435e-10, 1.86710802814157e-10, 1.70635273863534e-10, 1.55930687980064e-10, 1.42481306624186e-10, 1.30181057790878e-10, 1.18932737801671e-10, 1.08647278209201e-10, 9.92430725748863e-11, 9.06453582950663e-11, 8.27856490334434e-11, 7.56012136704299e-11, 6.90345980053579e-11, 6.30331857474433e-11, 5.75487956079478e-11, 5.25373115608672e-11, 4.79583435743883e-11, 4.37749163318197e-11, 3.99531836601083e-11, 3.64621665677439e-11, 3.32735129630055e-11, 3.03612772792429e-11, 
2.77017183772596e-11, 2.52731142267888e-11, 2.30555919904645e-11, 2.10309722453443e-11, 1.91826261797451e-11, 1.74953446975830e-11, 1.59552184492373e-11, 1.45495278877978e-11, 1.32666425229607e-11, 1.20959286123238e-11, 1.10276645918872e-11, 1.00529636046009e-11, 9.16370253824348e-12, 8.35245703211887e-12, 7.61244195636034e-12, 6.93745690835701e-12, 6.32183630823821e-12, 5.76040370974443e-12, 5.24842997441282e-12, 4.78159498603914e-12, 4.35595260905192e-12, 3.96789861893487e-12, 3.61414135533970e-12, 3.29167486919849e-12, 2.99775435412426e-12, 2.72987366981475e-12, 2.48574478117179e-12, 2.26327895153509e-12, 2.06056954190735e-12, 1.87587628041735e-12, 1.70761087761789e-12, 1.55432387362842e-12, 1.41469261268532e-12, 1.28751024942430e-12, 1.17167569925493e-12, 1.06618445255607e-12, 9.70120179176324e-13, 8.82647055917475e-13, 8.03002755355921e-13, 7.30492039560291e-13, 
6.64480907032201e-13, 6.04391245566386e-13, 5.49695947730361e-13, 4.99914449331670e-13, 4.54608654601190e-13, 4.13379214899274e-13, 3.75862130571052e-13, 3.41725648159969e-13, 3.10667427553834e-13, 2.82411955803873e-13, 2.56708186340823e-13, 2.33327384128352e-13, 2.12061158957008e-13, 1.92719670604243e-13, 1.75129990979628e-13, 1.59134609649920e-13, 1.44590070306053e-13, 1.31365726802484e-13, 1.19342608376890e-13, 1.08412384552649e-13, 9.84764210448520e-14, 8.94449187390810e-14, 8.12361284968988e-14, 7.37756351681349e-14, 6.69957047626884e-14, 6.08346892581635e-14, 5.52364839983536e-14, 5.01500330752442e-14, 4.55288784872365e-14, 4.13307492319387e-14, 3.75171868260434e-14, 3.40532040501677e-14, 3.09069739955730e-14, 2.80495467446096e-14, 2.54545912496319e-14, 2.30981601878877e-14, 2.09584757642051e-14, 1.90157346107966e-14, 1.72519300955857e-14, 1.56506904984806e-14, 
1.41971316501794e-14, 1.28777227514884e-14, 1.16801642038076e-14, 1.05932763842794e-14, 9.60689839298851e-15, 8.71179588528977e-15, 7.89957718055663e-15, 7.16261691001715e-15, 6.49398653148027e-15, 5.88739109818568e-15, 5.33711172323687e-15, 4.83795323054434e-15, 4.38519652833446e-15, 3.97455528244059e-15, 3.60213650413600e-15, 3.26440470150593e-15, 2.95814927457727e-15, 2.68045486289334e-15, 2.42867438017647e-15, 2.20040449438375e-15, 1.99346333303212e-15, 1.80587021332933e-15, 1.63582721456795e-15, 1.48170242656968e-15, 1.34201472284939e-15, 1.21541992072807e-15, 1.10069820297832e-15, 9.96742686841870e-16, 9.02549036511458e-16, 8.17206024509109e-16, 7.39886955899583e-16, 6.69841877024717e-16, 6.06390497499970e-16, 5.48915760638794e-16, 4.96858003320228e-16, 4.49709651641143e-16, 4.07010403543137e-16, 3.68342854018157e-16, 3.33328522514641e-16, 3.01624245822963e-16, 
2.72918903047290e-16, 2.46930442299818e-16, 2.23403181509686e-16, 2.02105358246818e-16, 1.82826905742816e-16, 1.65377434367030e-16, 1.49584399704446e-16, 1.35291440099940e-16, 1.22356868095946e-16, 1.10652301611695e-16, 1.00061422004550e-16, 9.04788473291716e-17, 8.18091101788785e-17, 7.39657304651144e-17, 6.68703743742468e-17, 6.04520915439038e-17, 5.46466232309370e-17, 4.93957749065471e-17, 4.46468473170557e-17, 4.03521205967734e-17, 3.64683865173660e-17, 3.29565244105674e-17, 2.97811167122010e-17, 2.69101004489523e-17, 2.43144513286369e-17, 2.19678974029264e-17, 1.98466595514452e-17, 1.79292162904333e-17, 1.61960906400940e-17, 1.46296569944614e-17, 1.32139661280568e-17, 1.19345866465128e-17, 1.07784613453433e-17, 9.73377708356471e-18, 8.78984690826589e-18, 7.93700328367971e-18, 7.16650138491662e-18, 6.47043151328614e-18, 5.84163977794677e-18, 5.27365626831808e-18, 
4.76063001400521e-18, 4.29727009460040e-18, 3.87879232126172e-18, 3.50087096597887e-18, 3.15959506343337e-18, 2.85142885480634e-18, 2.57317598320038e-18, 2.32194708690664e-18, 2.09513046990837e-18, 1.89036555908346e-18, 1.70551888483764e-18, 1.53866234662416e-18, 1.38805354722395e-18, 1.25211799998318e-18, 1.12943303162933e-18, 1.01871321998799e-18, 9.18797221060242e-19, 8.28635853641319e-19, 7.47281322095490e-19, 6.73877469171099e-19, 6.07650960951011e-19, 5.47903315286281e-19, 4.94003693444398e-19, 4.45382382298670e-19, 4.01524901266115e-19, 3.61966674434484e-19, 3.26288213964971e-19, 2.94110765971272e-19, 2.65092374707276e-19, 2.38924325090325e-19, 2.15327927385602e-19, 1.94051611316800e-19, 1.74868299982827e-19, 1.57573036780112e-19, 1.41980841083036e-19, 1.27924770746215e-19, 1.15254171584394e-19, 1.03833095879504e-19, 9.35388736783942e-20, 8.42608221960466e-20, 
7.58990800429806e-20, 6.83635542659414e-20, 6.15729693405857e-20, 5.54540082950074e-20, 4.99405370840484e-20, 4.49729041857525e-20, 4.04973081615272e-20, 3.64652266183270e-20, 3.28329006413784e-20, 2.95608693359800e-20, 2.66135496324475e-20, 2.39588569744872e-20, 2.15678629328980e-20, 1.94144861677392e-20, 1.74752135068077e-20, 1.57288482199461e-20, 1.41562828504629e-20, 1.27402942196854e-20, 1.14653584509271e-20, 1.03174840672955e-20, 9.28406140589761e-21, 8.35372676103255e-21, 7.51623982263034e-21, 6.76237311506426e-21, 6.08381226695129e-21, 5.47306605594524e-21, 4.92338527497562e-21, 4.42868955894882e-21, 3.98350139454904e-21, 3.58288661131831e-21, 3.22240072043320e-21, 2.89804052923144e-21, 2.60620051521272e-21, 2.34363349351671e-21, 2.10741515728752e-21, 1.89491211133899e-21, 1.70375305656048e-21, 1.53180281593656e-21, 1.37713892323882e-21, 1.23803052270097e-21, 
1.11291935259145e-21, 1.00040260780704e-21, 8.99217496659486e-22, 8.08227325122701e-22, 7.26408958140444e-22, 6.52841522335299e-22, 5.86696227761727e-22, 5.27227198350963e-22, 4.73763211527373e-22, 4.25700257249920e-22, 3.82494835551263e-22, 3.43657919602726e-22, 3.08749518511462e-22, 2.77373780530757e-22, 2.49174583205231e-22, 2.23831562241067e-22, 2.01056535642902e-22, 1.80590283944760e-22, 1.62199651227124e-22, 1.45674935097604e-22, 1.30827536955569e-22, 1.17487846694864e-22, 1.05503338553992e-22, 9.47368571267494e-23, 8.50650746231617e-23, 7.63771023428923e-23, 6.85732410109433e-23, 6.15638561468424e-23, 5.52683660096673e-23, 4.96143308971149e-23, 4.45366336906497e-23, 3.99767425424537e-23, 3.58820475044532e-23, 3.22052637147117e-23, 2.89038944908623e-23, 2.59397483419475e-23, 2.32785045061275e-23, 2.08893221587956e-23, 1.87444889194116e-23, 1.68191047211662e-23, 
1.50907975001406e-23, 1.35394675141714e-23, 1.21470574200775e-23, 1.08973455246789e-23, 9.77575988329724e-24, 8.76921115199901e-24, 7.86594230924399e-24, 7.05539355116851e-24, 6.32808083449424e-24, 5.67548669389855e-24, 5.08996209828481e-24, 4.56463823425460e-24, 4.09334721659120e-24, 3.67055082592605e-24, 3.29127646409623e-24, 2.95105959900749e-24, 2.64589204398625e-24, 2.37217548245679e-24, 2.12667970803467e-24, 1.90650510344917e-24, 1.70904892968672e-24, 1.53197503991280e-24, 1.37318667156972e-24, 1.23080200498613e-24, 1.10313220826706e-24, 9.88661716508526e-25, 8.86030518813154e-25, 7.94018249461053e-25, 7.11529900164414e-25, 6.37582988839591e-25, 5.71296036972452e-25, 5.11878222617703e-25, 4.58620089530984e-25, 4.10885205032873e-25, 3.68102670084185e-25, 3.29760394834536e-25, 2.95399061700499e-25, 2.64606705936534e-25, 2.37013850769252e-25, 2.12289140554628e-25, 
1.90135421160908e-25, 1.70286221941511e-25, 1.52502598301802e-25, 1.36570298032989e-25, 1.22297218333453e-25, 1.09511123805212e-25, 9.80575987388494e-26, 8.77982097192185e-26, 7.86088570270586e-26, 7.03782955064821e-26, 6.30068075402401e-26, 5.64050125460738e-26, 5.04927989988981e-26, 4.51983664130250e-26, 4.04573660026715e-26, 3.62121298923375e-26, 3.24109797844444e-26, 2.90076069219689e-26, 2.59605160192491e-26, 2.32325265844158e-26, 2.07903257306349e-26, 1.86040671782827e-26, 1.66470116933796e-26, 1.48952046952886e-26, 1.33271872045391e-26, 1.19237366947299e-26, 1.06676347653295e-26, 9.54345886897529e-27, 8.53739561120167e-27, 7.63707339575226e-27, 6.83141241770327e-27, 6.11049021220921e-27, 5.46542115119766e-27, 4.88824844591535e-27, 4.37184736181537e-27, 3.90983848560952e-27, 3.49651000394170e-27, 3.12674806048487e-27, 2.79597435456989e-27, 2.50009023085833e-27, 
2.23542658708717e-27, 1.99869899644640e-27, 1.78696750352907e-27, 1.59760060874240e-27, 1.42824300625326e-27, 1.27678668555358e-27, 1.14134504709844e-27, 1.02022971867448e-27, 9.11929791622333e-28, 8.15093225150955e-28, 7.28510193088837e-28, 6.51098170825066e-28, 5.81888581182388e-28, 5.20014836781769e-28, 4.64701633328764e-28, 4.15255363376753e-28, 3.71055533679951e-28, 3.31547081403350e-28, 2.96233495350729e-28, 2.64670658136731e-28, 2.36461333981269e-28, 2.11250234648217e-28, 1.88719603080459e-28, 1.68585260583059e-28, 1.50593069051806e-28, 1.34515764803292e-28, 1.20150125095403e-28, 1.07314432488750e-28, 9.58462058383897e-29, 8.56001699653937e-29, 7.64464389786305e-29, 6.82688908335463e-29, 6.09637130587033e-29, 5.44381016803265e-29, 4.86090972557428e-29, 4.34025436110490e-29, 3.87521563869419e-29, 3.45986898478353e-29, 3.08891916193882e-29, 2.75763361031806e-29, 
2.46178282876492e-29, 2.19758705432543e-29, 1.96166857678941e-29, 1.75100909451634e-29, 1.56291158017448e-29, 1.39496618086066e-29, 1.24501972705473e-29, 1.11114846961454e-29, 9.91633704074665e-30, 8.84939977370138e-30, 7.89695604202997e-30, 7.04675248995593e-30, 6.28784355087206e-30, 5.61045225840745e-30, 5.00584582919761e-30, 4.46622445425247e-30, 3.98462190072296e-30, 3.55481667343903e-30, 3.17125261762756e-30, 2.82896796236068e-30, 2.52353190998808e-30, 2.25098797137071e-30, 2.00780333133918e-30, 1.79082360448180e-30, 1.59723240907160e-30, 1.42451524750375e-30, 1.27042723578408e-30, 1.13296427306311e-30, 1.01033728554461e-30, 9.00949217855632e-31, 8.03374479627295e-31, 7.16340586032206e-31, 6.38711758744938e-31, 5.69474278578310e-31, 5.07723403212270e-31, 4.52651683250014e-31, 4.03538527554155e-31, 3.59740884657945e-31, 3.20684921209026e-31, 2.85858591064470e-31, 
2.54804999974730e-31, 2.27116480911494e-31, 2.02429304138801e-31, 1.80418954210450e-31, 1.60795913302120e-31, 1.43301896744481e-31, 1.27706492394886e-31, 1.13804160643269e-31, 1.01411556457142e-31, 9.03651389895989e-32, 8.05190379549427e-32, 7.17431492652540e-32, 6.39214353599040e-32, 5.69504082854627e-32, 5.07377759291980e-32, 4.52012339050584e-32, 4.02673874631523e-32, 3.58707894662611e-32, 3.19530819708729e-32, 2.84622302847381e-32, 2.53518395649740e-32, 2.25805450853769e-32, 2.01114682525408e-32, 1.79117312996148e-32, 1.59520243449835e-32, 1.42062191804862e-32, 1.26510247586159e-32, 1.12656798882638e-32, 1.00316791308363e-32, 8.93252831919366e-33, 7.95352650634576e-33, 7.08157149410234e-33, 6.30498639835365e-33, 5.61336498124999e-33, 4.99743372479619e-33, 4.44892883841347e-33, 3.96048658764155e-33, 3.52554550488378e-33, 3.13825919815636e-33, 2.79341861220243e-33, 
2.48638271985395e-33, 2.21301673176041e-33, 1.96963701098640e-33, 1.75296196677273e-33, 1.56006828010041e-33, 1.38835188360710e-33, 1.23549318078257e-33, 1.09942604503029e-33, 9.78310188840389e-34, 8.70506537625365e-34, 7.74555282298159e-34, 6.89156319934330e-34, 6.13151823318977e-34, 5.45510708240384e-34, 4.85314792424174e-34, 4.31746462329941e-34, 3.84077683946979e-34, 3.41660211488936e-34, 3.03916863728952e-34, 2.70333751845663e-34, 2.40453355250690e-34, 2.13868353104439e-34, 1.90216129247369e-34, 1.69173877208609e-34, 1.50454239920919e-34, 1.33801425874821e-34, 1.18987749778299e-34, 1.05810551435602e-34, 9.40894515930830e-35, 8.36639079883189e-35, 7.43910388399514e-35, 6.61436845824204e-35, 5.88086818292731e-35, 5.22853263827536e-35, 4.64840046334874e-35, 4.13249749456329e-35, 3.67372826295336e-35, 3.26577938924103e-35, 2.90303357514801e-35, 2.58049303143026e-35, 
2.29371130969616e-35, 2.03873261785956e-35, 1.81203779958987e-35, 1.61049624767497e-35, 1.43132310100796e-35, 1.27204114600395e-35, 1.13044690659260e-35, 1.00458046336407e-35, 8.92698592717507e-36, 7.93250861646073e-36, 7.04858353687240e-36, 6.26294737103280e-36, 5.56469418011608e-36, 4.94412549378752e-36, 4.39261691903935e-36, 3.90249945184537e-36, 3.46695387474528e-36, 3.07991680087163e-36, 2.73599708290912e-36, 2.43040144615972e-36, 2.15886833016435e-36, 1.91760903488008e-36, 1.70325536674168e-36, 1.51281306837346e-36, 1.34362039445928e-36, 1.19331126638493e-36, 1.05978250067917e-36, 9.41164661845009e-37, 8.35796139636565e-37, 7.42200094869426e-37, 6.59063957049037e-37, 5.85221191990228e-37, 5.19635088656657e-37, 4.61384342089172e-37, 4.09650233893329e-37, 3.63705233651530e-37, 3.22902864110622e-37, 2.86668690337280e-37, 2.54492308465176e-37, 2.25920223390370e-37, 
2.00549516991209e-37, 1.78022219321893e-37, 1.58020304903844e-37, 1.40261244846857e-37, 1.24494053190719e-37, 1.10495772671885e-37, 9.80683511815493e-38, 8.70358655743840e-38, 7.72420542845178e-38, 6.85481244728680e-38, 6.08308032260899e-38, 5.39806057039351e-38, 4.79002961352539e-38, 4.25035202341343e-38, 3.77135899835350e-38, 3.34624038468202e-38, 2.96894873467574e-38, 2.63411406227651e-38, 2.33696810633260e-38, 2.07327704320365e-38, 1.83928170809353e-38, 1.63164448896488e-38, 1.44740214980260e-38, 1.28392392260013e-38, 1.13887428089024e-38, 1.01017987294149e-38, 8.96000150793681e-39, 7.94701282915713e-39, 7.04832984147977e-39, 6.25107937375347e-39, 5.54383517630567e-39, 4.91645561551943e-39, 4.35993953763022e-39, 3.86629827199943e-39, 3.42844197039568e-39, 3.04007867991879e-39, 2.69562472592261e-39, 2.39012514012980e-39, 2.11918301027848e-39, 1.87889675307308e-39};

// Dickman's "rho" function; rho(x) == F(1/x)
double rho(double x) {
  if (x <= 1) { return 1; }
  if (x < 2)  { return 1 - log(x); }
  x = (x - 2) * 40;
  int pos = x;
  constexpr int SIZE = sizeof(rhotab)/sizeof(rhotab[0]);
  assert(pos + 1 < SIZE);
  double dummy;
  double f = modf(x, &dummy);
  // linear interpolation between rhotab[pos] and rhotab[pos+1]
  return rhotab[pos] * (1 - f) + rhotab[pos + 1] * f;
}

// Integrate a function from "a" to "b".
template <u32 STEPS = 31, typename Fun>
double integral(double a, double b, Fun f) {
  assert(b >= a);
  if (b <= a) { return 0; }

  double step = (b - a) / STEPS;
  double sum = 0;
  for (double x = a + step * .5; x < b; x += step) { sum += f(x); }
  return sum * step;
}

// See "Some Integer Factorization Algorithms using Elliptic Curves", R. P. Brent, page 3.
// https://maths-people.anu.edu.au/~brent/pd/rpb102.pdf
// Also "Speeding up Integer Multiplication and Factorization", A. Kruppa, chapter 5.3.3 (page 102).
double pFirstStage(double alpha) { return rho(alpha); }

double pSecondStage(double alpha, double beta) {
  return integral(1, beta, [alpha](double x) { return rho(alpha - x) / x; });
  
  // Equivalent:
  // return integral(alpha - beta, alpha - 1, [alpha](double t) {return rho(t)/(alpha-t); });
}

// Returns the probability of PM1(B1,B2) success for a Mersenne 2^exponent -1 already TF'ed to factoredUpTo.
std::pair<double, double> pm1(double exponent, u32 factoredUpTo, u32 B1, u32 B2) {
  // Mersenne factors have special form 2*k*p+1 for M(p)
  // so sustract log2(exponent) + 1 to obtain the magnitude of the "k" part.
  double takeAwayBits = log2(exponent) + 1;

  // We consider factors of bit-size up to BIT_END -- the total contribution of larger factors being negligeable.
  constexpr double BIT_END = 175;
  
  // We walk the bit-range [factoredUpTo, BIT_END] in steps of SLICE_WIDTH bits.
  constexpr double SLICE_WIDTH = 0.25;

  // The middle point of the slice is (2^n + 2^(n+SLICE_WIDTH))/2,
  // so log2(middle) is n + log2(1 + 2^SLICE_WIDTH) - 1.
  constexpr double SLICE_MIDDLE = log2(1 + exp2(SLICE_WIDTH)) - 1;
  
  const double bitsB1 = log2(B1);
  const double bitsB2 = log2(B2);  
  const double beta  = bitsB2 / bitsB1;
  assert(beta >= 1);

  double sum1 = 0;
  double sum2 = 0;
  
  for (double bitPos = factoredUpTo + SLICE_MIDDLE, alpha = (bitPos - takeAwayBits) / bitsB1; bitPos < BIT_END; bitPos += SLICE_WIDTH, alpha += SLICE_WIDTH / bitsB1) {
    double sliceProb = SLICE_WIDTH / bitPos;
    double p1 = pFirstStage(alpha) * sliceProb;
    double p2 = pSecondStage(alpha, beta) * sliceProb;
    double p = p1 + p2;
    sum1 += fma(p1, -sum1, p1);
    sum2 += fma(p,  -sum2, p);
  }
  return {sum1, sum2 - sum1};
}

// Approximation of the number of primes <= n.
// The correction term "-1.06" was determined experimentally to improve the approximation.
double primepi(double n) { return (n > 0) ? n / (log(n) - 1.06) : 0; }

double nPrimesBetween(double B1, double B2) { return (B2 <= B1) ? 0 : (primepi(B2) - primepi(B1)); }

// factorBias indicates how much a factor is desired, e.g.:
// factorBias == 1 indicates that a factor has the same value as a PRP "composite" status.
// factorBias == 2 indicates that a factor has double the value of a PRP "composite" status.
double work(double exponent, u32 factored, double B1, double B2, double factorBias, bool legacyP1) {
  const double factorP1 = legacyP1 ? 1.05 : (1.4 / 9);
  const double factorP2 = 0.7;
  
  auto [p1, p2] = pm1(exponent, factored, B1, B2);
  double iterationsP1 = 1.442 * B1;
  double workP1 = legacyP1 ? iterationsP1 * factorP1 : (iterationsP1 * (1 + factorP1));
  double workP2 = factorP2 * nPrimesBetween(B1, B2);

  double bonus = (factorBias - 1) * exponent;
  
  // The two alternatives below are equivalent (achieve exactly the same bounds)
  #if 1
  double workAfterP1 = p2 * (workP2 / 2) + (1 - p2) * (workP2 + exponent + bonus - (legacyP1 ? 0 : iterationsP1));
  double w = workP1 + (1 - p1) * workAfterP1;
  
  #else
  double workAfterP1 = p2 * (workP2 / 2 - bonus) + (1 - p2) * (workP2 + exponent - (legacyp1 ? 0 : iterationsP1));
  double w = workP1 - p1 * bonus + (1 - p1) * workAfterP1;
  #endif
  
  return w;
}

// Returns work for stage-1, stage-2 in the negative (no factor found) case, as a percent of one PRP test.
tuple<double, double> stageWork(double exponent, u32 factored, double B1, double B2, bool legacyP1) {
  const double factorP1 = legacyP1 ? 1.05 : (1.4 / 9);
  const double factorP2 = 0.7;
  double iterationsP1 = 1.442 * B1;
  double workP1 = iterationsP1 * factorP1;
  double workP2 = factorP2 * nPrimesBetween(B1, B2);
  return {workP1 / exponent, workP2 / exponent};
}

const constexpr double B1s[] = {0.5, 0.6, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.5, 1.7, 2, 2.5, 3, 3.5, 4, 4.5, 5, 6, 7, 8, 9, 10, 12, 15, 20, 25, 30};
const constexpr double B2s[] = {10, 15, 20, 25, 30, 35, 40, 45, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 180, 200, 220, 250, 300, 350, 400, 500, 600, 700, 800};

std::pair<double, double> scanBounds(double exponent, u32 factored, double factorBias, u32 fixedB1, u32 fixedB2, bool useLegacyP1 = false) {
  vector<double> b1s, b2s;
  
  if (fixedB1) {
    b1s.push_back(fixedB1);
  } else {
    for (auto b : B1s) { b1s.push_back(b * 1'000'000); }
  }
  
  if (fixedB2) {
    b2s.push_back(fixedB2);    
  } else {
    for (auto b : B2s) { b2s.push_back(b * 1'000'000); }
  }

  double best = 1e20;
  u32 bestB1 = 0, bestB2 = 0;
  for (u32 b2 : b2s) {
    for (u32 b1 : b1s) {
      if (b1 <= b2) {
        if (double w = work(exponent, factored, b1, b2, factorBias, useLegacyP1); w < best) {
          best = w;
          bestB1 = b1;
          bestB2 = b2;
        }
      }
    }
  }
  return {bestB1, bestB2};
}

static u32 parse(const string& s) {
  u32 len = s.size();
  assert(len);
  char c = s[len - 1];
  u32 multiple = (c == 'M' || c == 'm') ? 1'000'000 : ((c == 'K' || c == 'k') ? 1000 : 1);
  return atof(s.c_str()) * multiple;
}

void printBounds(double exponent, double factored, double B1, double B2, bool useLegacyP1 = false) {
  auto [p1, p2] = pm1(exponent, factored, B1, B2);
  auto [p3, p4] = pm1(exponent, factored, B2, B2);
  assert(p4 == 0);
  double p = p1 + p2;
  auto [w1, w2] = stageWork(exponent, factored, B1, B2, useLegacyP1);
  printf("B1=%4.1fM B2=%3.0fM | %.3f%% (%.3f%% + %.3f%%) | work %.3f%% (%.3f%% + %.3f%%) | B2/B1=%2.0f, misses %.2f%% of B2-smooth factors\n",
         B1/1'000'000, B2/1'000'000, p * 100, p1 * 100, p2 * 100, (w1 + w2) * 100, w1 * 100, w2 * 100, B2 / B1, (p3 - p) / p3 * 100);
}

int main(int argc, char*argv[]) {
  if (argc < 3) {
    printf(R"(Usage: %s <exponent> <factoredTo> [-legacy] [-B1 <B1>] [-B2 <B2>] [-bias <factor-bias>]
Examples:
%s 100M 76
%s 105M 76 -legacy
%s 102M 76 -B2 100M
%s 102M 77 -B1 3M
)", argv[0], argv[0], argv[0], argv[0], argv[0]);
    return 1;
  }
  
  u32 exponent = parse(argv[1]);
  u32 factored = parse(argv[2]);
  u32 B1 = 0, B2 = 0;
  double factorBias = -1;
  bool useLegacy = false;
  
  int pos = 3;
  while (pos < argc) {
    if ("-legacy"s == argv[pos]) {
      useLegacy = true;
      ++pos;
    } else if (pos + 1 < argc) {    
      if ("-B1"s == argv[pos]) {
        B1 = parse(argv[pos + 1]);
      } else if ("-B2"s == argv[pos]) {
        B2 = parse(argv[pos + 1]);
      } else if ("-bias"s == argv[pos]) {
        factorBias = atof(argv[pos + 1]);
        assert(factorBias > 0);
      }
      pos += 2;
    } else {
      printf("Unrecognized '%s'\n", argv[pos]);
      ++pos;
    }
  }

  if (B1 && B2) {
    printBounds(exponent, factored, B1, B2, useLegacy);
  } else {
    auto points = factorBias > 0 ? vector{factorBias} : vector{1.2, 2.0, 3.0, 4.0, 5.0, 6.0};
    for (double bias : points) {
      auto [bestB1, bestB2] = scanBounds(exponent, factored, bias, B1, B2, useLegacy);
      printBounds(exponent, factored, bestB1, bestB2, useLegacy);
    }
  }
}
