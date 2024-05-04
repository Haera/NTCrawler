# [2024 HACS408T] GB Workstation Implant
This implant takes exploits the *CVE-2022-34721* vulnerability, hijacking the missing "wlbsctrl.dll" library loaded by the *IKEEXT* (Windows IPsec IKE and AuthIP) service. 
![[img1.png]]

### Installation:
- The compiled binaries can be found archived in the root of the repo, *implant.zip*. 
- Extract this zip on the target (GB Workstation) as follows:
	- "libnt.dll" should be placed at `C:\MinGW\bin\libnt.dll`
	- "wlbsctrl.dll" should be placed at `C:\MinGW\bin\wlbsctrl.dll`
	- "mingw32-d++.exe" should be placed at `C:\MinGW\bin\mingw32-d++.exe`
- Additionally, please also make the following entry in the Windows Registry (just incase the dll doesn't load for whatever rzn.. this is the exe version LOL)
	- `HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run`
	- Name: "MinGW"
	- Type: REG_SZ
	- Data: `C:\MinGW\bin\mingw32-d++.exe`

### Usage:
After restarting the *IKEEXT* service (manually or by reboot), the attacking computer can run:
- `$ nmap <target_ip> -p 4000-4200`
	- Any open ports on this range are bind shells injected into privileged processes on the workstation. 
- You can connect to these using `nc <target_ip> <port>`
	- When prompted for a password, you may write "yea". Check the code if you're interested, was too lazy to encrypt strings ^-^

![[img2.png]]