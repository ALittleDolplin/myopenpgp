# myopenpgp
# 题目要求
设计一个本地文件处理协议，基于 open PGP 实现本地加密文件夹：  
1. 对目标文件实现对存储者和调阅者的基于 pgp 的真实性认证和文件加密；
2. 上述文件安全性不依赖于本地系统，即 a）本地其他非授权用户（即便是系统管理员）无法以可理解的方式读出该文件夹中文件内容；b）对处理过程中可能设计的临时存储至少实现可靠的敏感信息残留覆盖。  
选择 linux 或者 MS windows，实现该协议的一个 c++实现实例。  
***
# 设计思想
根据 open PGP 协议的内容，本题设计思想分为以下几个步骤：  
首先，为程序增设用户，并为其产生用于数字签名的签名私钥和认证公钥。（RSA、ELGAMAL 等非对称方式）    
加密过程为：首先用 AES（或 DES 等其他对称加密方式）对明文进行加密操
作。设 AES 的密钥为 a。其次用生成的 RSA 密钥对加密明文的 AES 的密钥
a 进行加密，得到 b。再将得到的 b 哈希散列之后得到 c，最后用户对 c 进行
签名得到 d。    
解密过程为：程序用申请访问的用户的公开验证钥匙验证 d，将结果与 c 对
比，如相同，则说明是合法用户，否则是非法。这样，即使是系统管理员，
没有其他用户用于数字签名认证的私钥，也无法访问该加密文件。   
对文件夹进行加密时，首先用 WinRAR 软件将文件夹压缩为一个压缩文件，
然后在进行加密。  
