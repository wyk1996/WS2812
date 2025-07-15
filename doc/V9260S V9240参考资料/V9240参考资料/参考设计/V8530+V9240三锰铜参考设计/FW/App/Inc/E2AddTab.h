#ifndef _E2ADDRTAB_H_
#define _E2ADDRTAB_H_


/***********************************E2地址排布定义**************************************/
//以下宏定义数据都为对于的数据在E2中的存储地址,实际数据长度可以从相邻两项的地址差得到,
//或可以查看本表E2排布定义的详细设计文档
/****************************************************************************************/
#define EEP_BAK_LEN     0x02C0

//下面是第0页地址
#define EEP_COMADD      0x0000  //通讯地址,6Bytes
#define EEP_METERNO     0x0006  //表号 ,6Bytes
#define EEP_ASSETNO     0x000C  //资产管理编码（ASCII码）
#define EEP_STANDUN     0x002C  //额定电压（ASCII码）
#define EEP_STANDIN     0x0032  //额定电流/基本电流（ASCII码）
#define EEP_MAXIN       0x0038  //最大电流（ASCII码）
                                
//下面是第1页地址                                
#define EEP_YGACCURACY  0x0040  //有功准确度等级（ASCII码）
#define EEP_DBWZXX      0x0044  //电表位置信息
#define EEP_METERMODEL  0x004F  //电表型号（ASCII码）
#define EEP_PDATE       0x0059  //生产日期（ASCII码）
#define EEP_PROTOCOLNO  0x0063  //协议版本号（ASCII码）
#define EEP_KHBH        0x0073  //客户编号（6byte）
#define EEP_YGCALC      0x0079  //有功组合方式特征字
#define EEP_IARBDM      0x007A  //调制型红外光口通信速率特征字
#define EEP_UARTBDM     0x007B  //通信口1通信速率特征字
#define EEP_PROTIME     0x007C  //编程有效时间(自扩)

//下面是第2页地址                                
#define EEP_JBMCKD      0x0080  //校表脉冲宽度(ms) 
#define EEP_METERRUNST1 0x0082  //电表运行状态字1
#define EEP_METERRUNST2 0x0084  //电表运行状态字2
#define EEP_METERRUNST3 0x0086  //电表运行状态字3
#define EEP_METERRUNST4 0x0088  //电表运行状态字4
#define EEP_METERRUNST5 0x008A  //电表运行状态字5
#define EEP_METERRUNST6 0x008C  //电表运行状态字6
#define EEP_METERRUNST7 0x008E  //电表运行状态字7
#define EEP_BSTIME      0x0090  //闭锁时间(自扩)
#define EEP_BSCOUNT     0x0092  //闭锁次数(自扩)
#define EEP_RELAYDLY    0x0093  //跳闸延时时间
#define EEP_TZDLVAL     0x0095  //跳闸电流阀值（自扩）
#define EEP_TZDLDELAY   0x0097  //跳闸电流延时（自扩）
#define EEP_DSFREEZ     0x0099  //定时冻结数据模式字
#define EEP_YDFREEZ     0x009A  //瞬时冻结数据模式字
#define EEP_SSFREEZ     0x009B  //约定冻结数据模式字
#define EEP_ZDFREEZ     0x009C  //整点冻结数据模式字
#define EEP_RFREEZ      0x009D  //日冻结数据模式字
#define EEP_CJBH        0x009E  //厂家编号(ASCII码 32字节) 

//下面是第3页地址

#define EEP_645PASSWD1  0x00C0  //第一级密码
#define EEP_645PASSWD2  0x00C3  //第二级密码
#define EEP_645PASSWD3  0x00C6  //第三级密码
#define EEP_645PASSWD4  0x00C9  //第四级密码
#define EEP_645PASSWD5  0x00CC  //第五级密码
#define EEP_645PASSWD6  0x00CF  //第六级密码
#define EEP_645PASSWD7  0x00D2  //第七级密码
#define EEP_645PASSWD8  0x00D5  //第八级密码
#define EEP_645PASSWD9  0x00D8  //第九级密码
#define EEP_CBR1        0x00DB  //每月第一结算日
#define EEP_CBR2        0x00DD  //每月第二结算日
#define EEP_CBR3        0x00DF  //每月第三结算日
#define EEP_MAX_LX_NUM  0x00E1  //自动循环显示屏数
#define EEP_LXTM        0x00E2  //每屏显示时间
#define EEP_EDEC        0x00E3  //显示电能小数位数
#define EEP_PDEC        0x00E4  //显示功率(最大需量显示位数)
#define EEP_MAX_JX_NUM  0x00E5  //按键循环显示屏数
#define EEP_SHOWERRPOS  0x00E6  //ERR错误字显示位置
#define EEP_POWUPJXTM   0x00E7  //有电下键显显示时间(自扩)
#define EEP_POWDWJXTM   0x00E8  //停电下键显显示时间(自扩)
#define EEP_POWOFFXSTM  0x00E9  //停电显示时间(自扩)
#define EEP_FULLSHOWTM  0x00EB  //全屏显示时间(自扩)
#define EEP_POWUPBGTM   0x00EC  //上电背光亮时间(自扩)
#define EEP_IRBGTM      0x00ED  //红外触发背光亮时间(自扩)
#define EEP_KEYBGTM     0x00EE  //按键触发背光亮时间(自扩)
#define EEP_ESAMERRBGTM 0x00EF  //ESAM错误背光亮时间(自扩)
#define EEP_LEDSFCKTM   0x00F0  //LED自检时间(自扩)
#define EEP_ZDFREEZTM   0x00F1  //整点冻结起始时间
#define EEP_ZDTMJG      0x00F6  //整点冻结时间间隔
#define EEP_DSFRZ       0x00F7  //定时冻结时间
#define EEP_RFREEZTM    0x00FB  //日冻结时间
#define EEP_DBYXTZZ     0x00FD  //电表运行特征字1

//下面是第4页地址
//RTC 参数
#define EEP_RTCTEMPOFFSET 0x0100    //RTC温度常温偏移
#define EEP_RTCFRQOFFSET  0x0101    //RTC晶体常温偏移
#define EEP_RTCDDTEMP     0x0105    //RTC顶点温度
#define EEP_RTCBVAL       0x0106    //RTC的B值
 
 
//下面是第5页地址 
//校表参数 
#define EEP_JBTOTAL     0x0140  //校表参数首地址
#define EEP_JBBCS       0x0140  //表常数
#define EEP_JBBCDY      0x0142  //标称电压
#define EEP_JBBCDL      0x0144  //标称电流

//下面是第6页地址 
//实时时钟备份
#define EEP_DATETIME    0x01C0  //当前时钟备份(自扩)
#define EEP_AUTOCAL     0x01D0  //自动校表比对数据

//下面是第7页地址
#define EEP_SQCNT           0x0200  //年时区数
#define EEP_RSDCNT          0x0201  //日时段表数
#define EEP_FSDCNT          0x0202  //费率时段数(每日切换数)
#define EEP_FEECNT          0x0203  //费率数
#define EEP_SZX             0x0204  //周休日状态字
#define EEP_ZXSD            0x0205  //周休日时段表
#define EEP_SFRZTIME        0x0206  //身份认证时效
#define EEP_JRCNT           0x0208  //公共假日数(不带年/带年)
#define EEP_SQQHTM          0x020A  //两套时区表切换时间
#define EEP_RSDQHTM         0x020F  //两套日时段表切换时间
#define EEP_PDIRTIME        0x0214  //功率反向事件判定延时时间
#define EEP_PDIRGATE        0x0215  //功率反向事件有功功率触发下限
                                    //0x0218保留  
#define EEP_CFSAVECFG       0x0219  //CF保存配置项（自扩）
#define EEP_CBRCFG          0x021A  //抄表日补冻配置项（自扩）
#define EEP_DJCFG           0x021B  //冻结补冻配置项（自扩）
#define EEP_PWLCFG          0x021C  //密码等级有效配置项（自扩）
#define EEP_SQSDCFG         0x021E  //时区和时段切换时间设置判别配置项（自扩）
#define EEP_ENSHOWCFG       0x021F  //电量显示格式配置项（自扩）
#define EEP_TMSHOWCFG       0x0220  //时间显示格式配置项（自扩）
#define EEP_PWOFFSHOWMDCFG  0x0221  //停电显示模式配置项（自扩）
#define EEP_JDQCKCFG        0x0222  //继电器检测方式配置项（自扩）
#define EEP_METERCLRCNT     0x0223  //电表清零次数（自扩）
#define EEP_METERCLFENGVAL  0x0224  //电表清零电量阀值（自扩）
#define EEP_PRODUCE         0x0228  //生产模式状态(自扩)
#define EEP_PRODUCETM       0x0229  //生产模式退出时间(自扩
#define EEP_MYZTS           0x022B  //密钥总条数
#define EEP_SWBAKNUM        0x022C  //软件备案号
#define EEP_MYZTZ           0x0234  //密钥状态字
#define EEP_ZDSBMSZ         0x0238  //主动上报模式字

//下面是第8页地址
//新增负荷记录
#define EEP_FHJLMSZ         0x0240  //负荷记录模式字
#define EEP_FHJLTIME        0x0241  //负荷记录起始时间
#define EEP_FHJLFLAG        0x0245  //负荷记录起始标志
#define EEP_FHJL1JG         0x0246  //第1类负荷记录间隔时间
#define EEP_FHJL2JG         0x0248  //第2类负荷记录间隔时间
#define EEP_FHJL3JG         0x024A  //第3类负荷记录间隔时间
#define EEP_FHJL4JG         0x024C  //第4类负荷记录间隔时间

//下面是第9页地址
//以下是当前电量部分
//#define EEP_EPS             0x0280  //当前有功电量数据

//下面10~19页是上面的备份地址

//下面是第20~28页地址
#define EEP_1SQ         0x0580  //第一套时区表首地址
#define EEP_1RSDS       (EEP_1SQ+E2_PROTECT_LEN)  //第一套日时段表首地址

#define EEP_1RSDS1      EEP_1RSDS
#define EEP_1RSDS2      (EEP_1RSDS1+E2_PROTECT_LEN)
#define EEP_1RSDS3      (EEP_1RSDS2+E2_PROTECT_LEN)
#define EEP_1RSDS4      (EEP_1RSDS3+E2_PROTECT_LEN)
#define EEP_1RSDS5      (EEP_1RSDS4+E2_PROTECT_LEN)
#define EEP_1RSDS6      (EEP_1RSDS5+E2_PROTECT_LEN)
#define EEP_1RSDS7      (EEP_1RSDS6+E2_PROTECT_LEN)
#define EEP_1RSDS8      (EEP_1RSDS7+E2_PROTECT_LEN)


//下面是第29~37页地址
#define EEP_2SQ         0x07C0  //第二套时区表首地址
#define EEP_2RSDS       (EEP_2SQ+E2_PROTECT_LEN)  //第二套日时段表首地址

#define EEP_2RSDS1      EEP_2RSDS
#define EEP_2RSDS2      (EEP_2RSDS1+E2_PROTECT_LEN)
#define EEP_2RSDS3      (EEP_2RSDS2+E2_PROTECT_LEN)
#define EEP_2RSDS4      (EEP_2RSDS3+E2_PROTECT_LEN)
#define EEP_2RSDS5      (EEP_2RSDS4+E2_PROTECT_LEN)
#define EEP_2RSDS6      (EEP_2RSDS5+E2_PROTECT_LEN)
#define EEP_2RSDS7      (EEP_2RSDS6+E2_PROTECT_LEN)
#define EEP_2RSDS8      (EEP_2RSDS7+E2_PROTECT_LEN)

#define TPLEN           (42)  //费率参数长度

//下面是第38页地址
#define EEP_CJRJBB      0x0A00  //厂家软件版本号(ASCII码)

//下面是第38页地址
#define EEP_CJYJBB      0x0A20  //厂家硬件版本号(ASCII码)

//下面是第39页地址                                
//下面是轮显表,键显表等
#define EEP_LXTABLE     0x0A40  //轮显表

//下面是第47页地址  
#define EEP_JXTABLE     0x0C40  //键显表
                                
//下面是第55页地址                                  
#define EEP_JRS         0x0E40  //公共假日日期表首地址

//下面是第59页地址 
#define EEP_POWDWENERGY     0x0F40      //掉电保存电量增量
#define EEP_POWDWCURFEE     0x0F56      //掉电时费率
#define EEP_POWDWCURTIME    0x0F57      //掉电时时间
#define EEP_POWDWCFCNT      0x0F5D      //掉电时CF数
#define EEP_POWDWCFENERGY   0x0F67      //掉电时CF对应能量数
#define EEP_PRGLEFTTM       0x0F71      //编程剩余时间(计时使用)(自扩)
#define EEP_PRODUCELEFTTM   0x0F73      //厂内时间（计时使用）(自扩)
#define EEP_PW2BSTM         0x0F75      //2级密码闭锁时间
#define EEP_PW4BSTM         0x0F77      //4级密码闭锁时间
#define EEP_POWERDWTM       0x0F79      //停电时间
#define EEP_STRELAY         0x0F7D      //拉合闸状态（自扩）
//#define EEP_ESAMKEYST       0x0EFF      //Esam密钥状态





//月度组合有功有功累计用电量
#define EEP_SYZHLYDL_PT     0x1100
#define EEP_SYZHLYDLADDR    0x1101


//下面是第61页空闲
//#define EEP_DDLXZ     0x1140        //大电流修正值
#define EEP_BPFPARA_VALUE   0x1140    //带通滤波器系数寄存器

//以下是当前电量部分
#define EEP_EPS             0x11c0  //当前有功电量数据
#define EEP_NEPS          0x1216  //当前无功电量数据


//下面是第62页地址
//历史电量,修改
//#define EEP_EPHIS_PT    0x11C0      //历史有功电量存储指针
//#define EEP_EPHIS       0x11C1      //历史有功电量在E2的起始地址
#define EEP_EPHIS_PT    0x16C0      //历史有功电量存储指针
#define EEP_EPHIS       0x16C1      //历史有功电量在E2的起始地址




//下面是第74页地址
//瞬时冻结
#define EEP_SSDJHIS_PT  0x1A40      //瞬时冻结指针存储指针
#define EEP_SSDJHISNO   0x1A41      //瞬时冻结总次数
#define EEP_SSDJLHIS    0x1A44      //瞬时冻结指针数据在E2中的起始地址

//下面是第77页地址
//约定冻结
//时区
#define EEP_SQDJHIS_PT  0x1B00      //时区表切换冻结指针存储指针
#define EEP_SQDJHISNO   0x1B01      //时区表切换冻结总次数
#define EEP_SQDJLHIS    0x1B04      //时区表切换冻结指针数据在E2中的起始地址

//下面是第79页地址
//时段
#define EEP_RSDJHIS_PT  0x1B80      //日时段表切换冻结指针存储指针
#define EEP_RSDJHISNO   0x1B81      //日时段表切换冻结总次数
#define EEP_RSDJLHIS    0x1B84      //日时段表切换冻结指针数据在E2中的起始地址

//下面是第81页地址
//日冻结
#define EEP_RDJHIS_PT   0x1C00      //日冻结指针存储指针
#define EEP_RDJHISNO    0x1C01      //日冻结冻结总次数
#define EEP_RDJLHIS     0x1C04      //日冻结指针数据在E2中的起始地址

//下面是第129页地址
//整点冻结
#define EEP_ZDDJHIS_PT  0x2800      //整点冻结指针存储指针
#define EEP_ZDDJHISNO   0x2801      //整点冻结总次数
#define EEP_ZDDJLHIS    0x2804      //整点冻结指针数据在E2中的起始地址

//下面是第181页地址
//定时冻结
#define EEP_DSDJHIS_PT  0x3500      //定时冻结指针存储指针
#define EEP_DSDJHISNO   0x3501      //定时冻结总次数
#define EEP_DSDJLHIS    0x3504      //定时冻结指针数据在E2中的起始地址


//下面是第227页地址
//电表清零记录
#define EEP_DBQLETNO    0x4100      //电表清零总次数
#define EEP_DBQLET_PT   0x4103      //电表清零记录指针
#define EEP_DBQLET      0x4104      //电表清零记录在E2中的起始地址

//下面是第230页地址
//事件清零记录
#define EEP_SJQLETNO    0x41C0      //事件清零总次数
#define EEP_SJQLET_PT   0x41C3      //事件清零记录指针
#define EEP_SJQLET      0x41C4      //事件清零记录在E2中的起始地址

//下面是第233页地址
//校时记录
#define EEP_JSETNO      0x4280      //校时总次数
#define EEP_JSET_PT     0x4283      //校时记录指针
#define EEP_JSET        0x4284      //校时记录在E2中的起始地址

//下面是第236页地址
//编程记录
#define EEP_BCETNO      0x4340      //编程总次数
#define EEP_BCET_PT     0x4343      //编程记录指针
#define EEP_BCET        0x4344      //编程记录在E2中的起始地址

//下面是第244页地址
//掉电记录
#define EEP_DDETNO      0x4540      //掉电总次数
#define EEP_DDET_PT     0x4543      //掉电记录指针
#define EEP_DDET        0x4544      //掉电记录在E2中的起始地址

//下面是第246页地址
//开表盖记录
#define EEP_KBGETNO     0x45C0      //开表盖总次数
#define EEP_KBGET_PT    0x45C3      //开表盖记录指针
#define EEP_KBGET       0x45C4      //开表盖记录在E2中的起始地址


//下面是第251页地址
//电源异常事件记录
#define EEP_DYYCNO      0x4700      //电源异常事件总次数
#define EEP_DYYC_PT     0x4703      //电源异常事件记录指针
#define EEP_DYYC        0x4704      //电源异常事件记录在E2中的起始地址


//下面是第255页地址
//时段表编程记录
#ifdef SQSDPRLOG
#define EEP_RSDXGNO     0x4800      //日时段更改记录次数            3个字节
#define EEP_RSDXGJL_PT  0x4803      //日时段更改记录指针地址        1个字节
#define EEP_RSDXGJL     0x4804      //日时段更改记录最近10次首地址  530个字节


//下面是第264页地址
//时区表编程记录
#define EEP_SQXGNO      0x4a40      //时区表更改记录次数            3个字节
#define EEP_SQXGJL_PT   0x4a43      //时区表更改记录指针地址        1个字节
#define EEP_SQXGJL      0x4a44      //时区表更改记录最近10次首地址  530个字节
#endif

////下面是第339页地址
////周休日编程记录
//#ifdef ZXRPRLOG
//#define EEP_ZXRXGNO     0x54C0      //周休日更改记录次数            3个字节
//#define EEP_ZXRXGJL_PT  0x54C3      //周休日更改记录指针地址        1个字节
//#define EEP_ZXRXGJL     0x54C4      //周休日更改记录最近10次首地址  11*10=110个字节
//#endif

//下面是第273页地址
//有功组合字编程记录
#ifdef YGZHZPRLOG
#define EEP_YGZHETNO    0x4c80      //有功组合方式字更改记录次数            3个字节有
#define EEP_YGZHET_PT   0x4c83      //有功组合方式字更改记录指针地址        1个字节
#define EEP_YGZHET      0x4c84      //有功组合方式字更改记录最近10次首地址  11*10=110个字节
#endif

//下面是第275页地址
//结算日编程记录
#ifdef JSRPRLOG
#define EEP_JSRETNO     0x4d00      //结算日更改记录次数            3个字节
#define EEP_JSRET_PT    0x4d03      //结算日更改记录指针地址        1个字节
#define EEP_JSRET       0x4d04      //结算日更改记录最近10次首地址  16*10=160个字节
#endif

//下面是第278页地址
//结算时间记录
#define EEP_JSTIME_PT   0x4dC0      //结算时间记录指针
#define EEP_JSTIME      0x4dC1      //结算时间记录起始地址



//下面是第279页地址
//拉闸记录
#define EEP_LZETNO      0x4e00      //拉闸总次数
#define EEP_LZET_PT     0x4e03      //拉闸记录指针
#define EEP_LZET        0x4e04      //拉闸记录在E2中的起始地址


//下面是第282页地址
//合闸记录
#define EEP_HZETNO      0x4eC0      //合闸总次数
#define EEP_HZET_PT     0x4eC3      //合闸记录指针
#define EEP_HZET        0x4eC4      //合闸记录在E2中的起始地址


//下面是第285页地址
//密钥跟新记录
#define EEP_MYGXNO      0x4f80      //密钥跟新总次数
#define EEP_MYGX_PT     0x4f83      //密钥跟新记录指针
#define EEP_MYGXET      0x4f84      //密钥跟新记录在E2中的起始地址


//下面是第288页地址
//负荷开关误动作记录
#define EEP_FHKGNO      0x5040      //负荷开关误动作总次数
#define EEP_FHKG_PT     0x5043      //负荷开关误动作记录指针
#define EEP_FHKGET      0x5044      //负荷开关误动作记录在E2中的起始地址


//下面是第293页地址
//总负荷记录
#define EEP_ZFHETNO     0x5180      //总负荷次数
#define EEP_ZFHET_PT    0x5183      //总负荷指针
#define EEP_ZFHET       0x5185      //总负荷最近1次记录首地址


//下面是第343页地址
//第1类负荷记录
#define EEP_FHET1NO     0x5e00      //第1类负荷次数
#define EEP_FHET1_PT    0x5e03      //第1类负荷指针
#define EEP_FHET1       0x5e05      //第1类负荷最近1次记录首地址


//下面是第398页地址
//第2类负荷记录
#define EEP_FHET2NO     0x6bC0      //第2类负荷次数
#define EEP_FHET2_PT    0x6bC3      //第2类负荷指针
#define EEP_FHET2       0x6bC5      //第2类负荷最近1次记录首地址


//下面是第435页地址
//第3类负荷记录
#define EEP_FHET3NO    0x7500      //第3类负荷次数
#define EEP_FHET3_PT   0x7503      //第3类负荷指针
#define EEP_FHET3      0x7504      //第3类负荷最近1次记录首地址


//下面是第467页地址
//第4类负荷记录
#define EEP_FHET4NO    0x7d00      //第4类负荷次数
#define EEP_FHET4_PT   0x7d03      //第4类负荷指针
#define EEP_FHET4      0x7d05      //第4类负荷最近1次记录首地址


//下面是第526页地址
//密钥密文36字节*32条
#define EEP_MYGX_CMD   0x8bC0      //密钥密文首地址，36*32=1152字节


#endif

