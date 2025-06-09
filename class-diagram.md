+----------------+       +----------------+        +-------------------+
|     System     |       | Authentication |        |    UserManager    |
+----------------+       |    Manager     |        +-------------------+
| -running       |       +----------------+        | -users            |
| -currentUser   |<>---->| -userManager   |<------>| -dataFilePath     |
| -authManager   |       | -otpManager    |        | -otpManager       |
| -userManager   |       +----------------+        +-------------------+
| -walletManager |       | +login()       |        | +registerUser()   |
| -txManager     |       | +logout()      |        | +createUserByAdmin() 
| -otpManager    |       | +authenticate()|        | +searchUsers()    |
+----------------+       +----------------+        | +getAllUsers()    |
| +initialize()  |                                 | +loadData()       |
| +run()         |                                 | +saveData()       |
| +showMenu()    |                                 +-------------------+
+----------------+                                      ^
        ^                                              /|\
        |                                               |
        | uses                                          |
        |                                               |
+----------------+        +----------------+        +-------------------+
|      User      |        |      OTP       |        |  PasswordManager  |
+----------------+        |    Manager     |        +-------------------+
| -username      |        +----------------+        | +hashPassword()   |
| -passwordHash  |<-------| -otpStore      |        | +verifyPassword() |
| -fullName      |        | -otpPurpose    |        | +generateRandom   |
| -email         |        +----------------+        |  Password()       |
| -phoneNumber   |        | +generateOTP() |        | +isStrongPassword()|
| -address       |        | +verifyOTP()   |        +-------------------+
| -isAdmin       |        | +sendOTP()     |                 ^
| -walletId      |        +----------------+                /|\
| -pendingChanges|                 ^                         |
+----------------+                /|\                        |
| +getUsername() |                 |                         |
| +getPassword() |                 |                         |
| +setFullName() |                 |                         |
+----------------+                 |                         |
        ^                          |                         |
       /|\                         |                         |
        |                          |                         |
        |                          |                         |
+----------------+        +----------------+        +-------------------+
|     Wallet     |        | Transaction    |        | TransactionManager|
+----------------+        |    Manager     |        +-------------------+
| -walletId      |<-------+----------------+------->| -transactions     |
| -ownerUsername |        | -transactions  |        | -walletManager    |
| -balance       |        | -walletManager |        | -otpManager       |
| -isMasterWallet|        | -otpManager    |        | -dataFilePath     |
+----------------+        +----------------+        +-------------------+
| +getWalletId() |        | +initiate      |        | +getUserBalance() |
| +getBalance()  |        |  Transaction() |        | +getUserTx        |
| +isMaster()    |        | +confirm       |        |  History()        |
+----------------+        |  Transaction() |        | +loadData()       |
        ^                 | +addPoints     |        | +saveData()       |
       /|\                |  FromMaster()  |        +-------------------+
        |                 +----------------+
        |                        ^
        |                       /|\
        |                        |
+----------------+        +----------------+
| WalletManager  |        |  Transaction   |
+----------------+        +----------------+
| -wallets       |<-------| -transactionId |
| -userWallets   |        | -sourceWalletId|
| -masterWalletId|        | -destWalletId  |
| -dataFilePath  |        | -amount        |
+----------------+        | -timestamp     |
| +createWallet()|        | -status        |
| +getWallet()   |        | -description   |
| +userHasWallet()|       +----------------+
| +walletExists()|        | +getStatus()   |
| +createMaster  |        | +getAmount()   |
|  Wallet()      |        | +setStatus()   |
| +loadData()    |        | +serialize()   |
| +saveData()    |        +----------------+
+----------------+