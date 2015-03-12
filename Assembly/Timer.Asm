.386 
.model flat,stdcall
option casemap:none
WinMain proto :DWORD,:DWORD,:DWORD,:DWORD
include windows.inc
include user32.inc
include kernel32.inc
includelib user32.lib
includelib kernel32.lib
;--------------------equ-----------------------
IDD_DLG equ 1000
IDC_BTN1 equ 1001
IDC_BTN2 equ 1002
IDC_STC1 equ 1003
IDC_STC2 equ 1004
IDC_STC3 equ 1005
IDM_SZ equ 10001
IDM_JSQ equ 10002
IDM_EXIT equ 10003
IDM_ABOUT equ 10004
;-----------------定义数据段--------------
.data
zhuangtai1 db 0			;时钟进程状态
zhuangtai2 db 0			;按钮1开始暂停键的状态和计数器线程运行挂起状态
aboutCaption db "关于",0
aboutText db "计时器+时钟",0
kaishi db "开始",0
zanting db "暂停",0
min db 0			;计时器时间
s db 0
ms dw 0
.data?
hInstance HINSTANCE ?
hDlg dd ?
hThread1 dd ?
hThread2 dd ?
;-----------------定义代码段--------------
.code
start:
	invoke GetModuleHandle,NULL
	mov hInstance,eax
	mov  eax,OFFSET ThreadProc1
	invoke CreateThread,NULL,0,eax,NULL,CREATE_SUSPENDED,NULL
	mov hThread1,eax
	mov  eax,OFFSET ThreadProc2
	invoke CreateThread,NULL,0,eax,NULL,CREATE_SUSPENDED,NULL
	mov hThread2,eax
	invoke WinMain,hInstance,NULL,NULL,SW_SHOWDEFAULT
	invoke CloseHandle,hThread1		;删除的线程资源
	invoke CloseHandle,hThread2
	invoke ExitProcess,eax
;-----------------窗口过程-------------------
WndProc proc hWnd:HWND,uMsg:UINT,wParam:WPARAM,lparam:LPARAM
	.if uMsg==WM_DESTROY
		invoke PostQuitMessage,NULL
	.elseif uMsg==WM_CLOSE
		invoke DestroyWindow,hWnd
	.elseif uMsg==WM_COMMAND
		mov eax,wParam
		.if ax==IDC_BTN1
			xor zhuangtai2,1			
			.if zhuangtai2
				invoke ResumeThread,hThread2
				invoke SetDlgItemText,hWnd,IDC_BTN1,addr zanting
			.else
				invoke SetDlgItemText,hWnd,IDC_BTN1,addr kaishi
			.endif
		.elseif ax==IDC_BTN2
			invoke SuspendThread,hThread2
			mov min,0
			mov s,0
			mov ms,0
			invoke SetDlgItemInt,hDlg,IDC_STC1,0,FALSE
			invoke SetDlgItemInt,hDlg,IDC_STC2,0,FALSE
			invoke SetDlgItemInt,hDlg,IDC_STC3,0,FALSE
			invoke ResumeThread,hThread2
		.elseif ax==IDM_SZ
			mov zhuangtai2,0
			invoke ResumeThread,hThread1
			invoke GetDlgItem,hWnd,IDC_BTN1
			invoke ShowWindow,eax,SW_HIDE
			invoke GetDlgItem,hWnd,IDC_BTN2
			invoke ShowWindow,eax,SW_HIDE
			mov zhuangtai1,1		
		.elseif ax==IDM_JSQ
			.if zhuangtai1
				invoke SuspendThread,hThread1
				mov zhuangtai1,0
			.endif
				mov zhuangtai2,0
			mov min,0
			mov s,0
			mov ms,0
			invoke SetDlgItemInt,hDlg,IDC_STC1,0,FALSE
			invoke SetDlgItemInt,hDlg,IDC_STC2,0,FALSE
			invoke SetDlgItemInt,hDlg,IDC_STC3,0,FALSE
			invoke SetDlgItemText,hWnd,IDC_BTN1,addr kaishi		
			invoke GetDlgItem,hWnd,IDC_BTN1
			invoke ShowWindow,eax,SW_SHOWNORMAL
			invoke GetDlgItem,hWnd,IDC_BTN2
			invoke ShowWindow,eax,SW_SHOWNORMAL
		.elseif ax==IDM_EXIT
			invoke DestroyWindow,hWnd
		.elseif ax==IDM_ABOUT
			invoke MessageBox,hWnd,addr aboutText,addr aboutCaption,MB_OK
		
		.endif		
	.endif		
	xor eax,eax
	ret
WndProc endp
;-----------------窗口创建---------------------
WinMain proc hInst:HINSTANCE,hpreInst:HINSTANCE,CmdLine:LPSTR,CmdShow:DWORD
	LOCAL msg:MSG
	invoke CreateDialogParam,hInst,IDD_DLG,NULL,WndProc,NULL
	mov hDlg,eax
	invoke ShowWindow,hDlg,SW_SHOWNORMAL
	invoke UpdateWindow,hDlg
;-----------------消息循环------------------------
.while TRUE
	invoke GetMessage,ADDR msg,NULL,0,0
	.break .if(!eax)
	invoke IsDialogMessage,hDlg,ADDR msg
	.if eax==FALSE
		invoke TranslateMessage,ADDR msg
		invoke DispatchMessage,ADDR msg
	.endif
.endw
mov eax,msg.wParam
ret
WinMain endp
;-----------------时钟线程函数------------------
ThreadProc1 PROC lpParameter:DWORD
	LOCAL time:SYSTEMTIME
	.while TRUE
	invoke GetLocalTime,addr time
	invoke SetDlgItemInt,hDlg,IDC_STC1,time.wHour,FALSE
	invoke SetDlgItemInt,hDlg,IDC_STC2,time.wMinute,FALSE
	invoke SetDlgItemInt,hDlg,IDC_STC3,time.wSecond,FALSE
	invoke Sleep,500	
	.endw
	ret
ThreadProc1 endp
;-----------------计时器线程函数-----------------
ThreadProc2 PROC lpParameter:DWORD
	LOCAL chushu:BYTE
	LOCAL a:DWORD
	mov chushu,10
A:
	invoke GetTickCount
	mov a,eax
	.while TRUE
	invoke Sleep,35
	.if !zhuangtai2
		invoke SuspendThread,hThread2
		jmp A
	.endif
	invoke GetTickCount
	mov ecx,eax
	sub eax,a
	mov a,ecx
	add ms,ax	
	.if ms>999
		sub ms,1000
		inc s
		.if s==60
			mov s,0
			inc min
			invoke SetDlgItemInt,hDlg,IDC_STC1,min,FALSE
		.endif
		invoke SetDlgItemInt,hDlg,IDC_STC2,s,FALSE
	.endif
	invoke SetDlgItemInt,hDlg,IDC_STC3,ms,FALSE
	.endw
	ret
ThreadProc2 endp
end start
	
	

