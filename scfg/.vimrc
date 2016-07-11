" An example for a vimrc file.
"
" Maintainer:	Bram Moolenaar <Bram@vim.org>
" Last change:	2001 Jul 18
"
" To use it, copy it to
"     for Unix and OS/2:  ~/.vimrc
"	      for Amiga:  s:.vimrc
"  for MS-DOS and Win32:  $VIM\_vimrc
"	    for OpenVMS:  sys$login:.vimrc

set nu
set encoding=utf-8
set fileencodings=utf-8,gbk,gb2312,gb18030,ucs-bom,latin1

set tags=tags;
set shiftwidth=4 
set ts=4
"set nu
set vb t_vb=
" When started as "evim", evim.vim will already have done these settings.
if v:progname =~? "evim"
  finish
endif

" Use Vim settings, rather then Vi settings (much better!).
" This must be first, because it changes other options as a side effect.
set nocompatible

" allow backspacing over everything in insert mode
set backspace=indent,eol,start

set autoindent		" always set autoindenting on
set nobackup		" do not keep a backup file, use versions instead
"if has("vms")
"  set nobackup		" do not keep a backup file, use versions instead
"else
"  set backup		" keep a backup file
"endif
set history=50		" keep 50 lines of command line history
set ruler		" show the cursor position all the time
set showcmd		" display incomplete commands
set incsearch		" do incremental searching

set nobackup		" do not keep a backup file, use versions instead

" For Win32 GUI: remove 't' flag from 'guioptions': no tearoff menu entries
" let &guioptions = substitute(&guioptions, "t", "", "g")

" Don't use Ex mode, use Q for formatting
map Q gq

" Make p in Visual mode replace the selected text with the "" register.
vnoremap p <Esc>:let current_reg = @"<CR>gvs<C-R>=current_reg<CR><Esc>

" This is an alternative that also works in block mode, but the deleted
" text is lost and it only works for putting the current register.
"vnoremap p "_dp

" Switch syntax highlighting on, when the terminal has colors
" Also switch on highlighting the last used search pattern.
if &t_Co > 2 || has("gui_running")
  syntax on
  set hlsearch
endif

" Only do this part when compiled with support for autocommands.
if has("autocmd")

  " Enable file type detection.
  " Use the default filetype settings, so that mail gets 'tw' set to 72,
  " 'cindent' is on in C files, etc.
  " Also load indent files, to automatically do language-dependent indenting.
  filetype plugin indent on

  " For all text files set 'textwidth' to 78 characters.
  autocmd FileType text setlocal textwidth=78

  " When editing a file, always jump to the last known cursor position.
  " Don't do it when the position is invalid or when inside an event handler
  " (happens when dropping a file on gvim).
  autocmd BufReadPost *
    \ if line("'\"") > 0 && line("'\"") <= line("$") |
    \   exe "normal g`\"" |
    \ endif

endif " has("autocmd")

"if has("cscope")
"	let current = "."
"	let num = 1
"	while num < 20 
"		if filereadable(current . "/cscope.out")
"			let $CSCOPE_DB = current . "/cscope.out"
"			cs add $CSCOPE_DB
"			break
"		else
"			let current = current . "/.."
"			let num = num + 1
"		endif
"	endwhile
"endif
"
set cst
set csto=1
set cscopequickfix=s-,c-,d-,i-,t-,e-,f-
" cs add /home/yangxl/readcode/cscope-kernel/cscope.out
" cs add /home/yangxl/readcode/cscope-app/cscope.out
let Tlist_Enable_Fold_Column = 0
let Tlist_WinWidth = 30
let Tlist_Show_One_File = 1
set tabstop=4
"nmap <F2> :cs find d <C-R><C-W><CR>
"nmap <F3> :cs find c <C-R><C-W><CR>
"nmap <F4> :cs find t <C-R><C-W><CR>
"nmap <F5> :cs find e <C-R><C-W><CR>
"nmap <F6> :cs find f <C-R><C-W><CR>
"nmap <F7> :cs find i <C-R><C-W><CR>
nnoremap <F9> :TlistToggle <CR>

nmap <F2> :cs find c <C-R>=expand("<cword>")<CR><CR>
nmap <F3> :cs find s <C-R>=expand("<cword>")<CR><CR>
nmap <F4> :cs find g <C-R>=expand("<cword>")<CR><CR>
nmap <C-n> :cn<CR>
nmap <C-p> :cp<CR>
"nmap  :cs find t <C-R>=expand("<cword>")<CR><CR>
"nmap  :cs find e <C-R>=expand("<cword>")<CR><CR>
"nmap  :cs find f <C-R>=expand("<cfile>")<CR><CR>
"nmap  :cs find i ^<C-R>=expand("<cfile>")<CR>$<CR>
"nmap  :cs find d <C-R>=expand("<cword>")<CR><CR>

" Using 'CTRL-spacebar' then a search type makes the vim window
" split horizontally, with search result displayed in
" the new window.

"nmap <C-Space>s :scs find s <C-R>=expand("<cword>")<CR><CR>
"nmap <C-Space>g :scs find g <C-R>=expand("<cword>")<CR><CR>
"nmap <C-Space>c :scs find c <C-R>=expand("<cword>")<CR><CR>
"nmap <C-Space>t :scs find t <C-R>=expand("<cword>")<CR><CR>
"nmap <C-Space>e :scs find e <C-R>=expand("<cword>")<CR><CR>
"nmap <C-Space>f :scs find f <C-R>=expand("<cfile>")<CR><CR>
"nmap <C-Space>i :scs find i ^<C-R>=expand("<cfile>")<CR>$<CR>
"nmap <F2> :scs find d <C-R>=expand("<cword>")<CR><CR>

" Hitting CTRL-space *twice* before the search type does a vertical
" split instead of a horizontal one

"nmap <C-Space><C-Space>s
        \:vert scs find s <C-R>=expand("<cword>")<CR><CR>
"nmap <C-Space><C-Space>g
        \:vert scs find g <C-R>=expand("<cword>")<CR><CR>
"nmap <C-Space><C-Space>c
        \:vert scs find c <C-R>=expand("<cword>")<CR><CR>
"nmap <C-Space><C-Space>t
        \:vert scs find t <C-R>=expand("<cword>")<CR><CR>
"nmap <C-Space><C-Space>e
        \:vert scs find e <C-R>=expand("<cword>")<CR><CR>
"nmap <C-Space><C-Space>i
        \:vert scs find i ^<C-R>=expand("<cfile>")<CR>$<CR>
"nmap <F2> 
        \:vert scs find d <C-R>=expand("<cword>")<CR><CR>

if v:lang =~ "utf8$" || v:lang =~ "UTF-8$"
   set fileencodings=utf-8,latin1
endif

set nocompatible	" Use Vim defaults (much better!)
set bs=indent,eol,start		" allow backspacing over everything in insert mode
"set ai			" always set autoindenting on
"set backup		" keep a backup file
set viminfo='20,\"50	" read/write a .viminfo file, don't store more
			" than 50 lines of registers
set history=50		" keep 50 lines of command line history
set ruler		" show the cursor position all the time

" Only do this part when compiled with support for autocommands
if has("autocmd")
  augroup redhat
    " In text files, always limit the width of text to 78 characters
    autocmd BufRead *.txt set tw=78
    " When editing a file, always jump to the last cursor position
    autocmd BufReadPost *
    \ if line("'\"") > 0 && line ("'\"") <= line("$") |
    \   exe "normal! g'\"" |
    \ endif
  augroup END
endif

if has("cscope") && filereadable("/usr/bin/cscope")
   set csprg=/usr/bin/cscope
   set csto=0
   set cst
   set nocsverb
   " add any database in current directory
   if filereadable("cscope.out")
      cs add cscope.out
   " else add database pointed to by environment
   elseif $CSCOPE_DB != ""
      cs add $CSCOPE_DB/cscope.out
   endif
   set csverb
endif

" Switch syntax highlighting on, when the terminal has colors
" Also switch on highlighting the last used search pattern.
if &t_Co > 2 || has("gui_running")
  syntax on
  set hlsearch
endif

if &term=="xterm"
     set t_Co=8
     set t_Sb=[4%dm
     set t_Sf=[3%dm
endif
set nu
set relativenumber
set autoindent
set tags=/root/cloud/tags
"set tags=/root/nginx-1.8.0/tags
set tabstop=4
set softtabstop=4
set shiftwidth=4
set cst
set csto=0
nmap <F5> :make<cr><cr>:cw 7<cr>:cd -<cr>
nmap <F6> :cclose<cr>:cn<cr>
nmap <S-F6> :cclose<cr>:cp<cr>
nmap <F7> :cd $CSCOPE_DB<cr>
imap <F5> <ESC>:make<cr><cr>:cw 7<cr>:cd -<cr>
imap <F7> <ESC>:cd $CSCOPE_DB<cr>
imap <F8> <END><cr>printk("[%s][%d]-[%s]\n",__FILE__,__LINE__,__func__);
nmap <F8> i<END><cr>printk("[%s][%d]-[%s]\n",__FILE__,__LINE__,__func__); <ESC>
imap <S-F8> <END><cr>printf("[%s][%d]-[%s]\n",__FILE__,__LINE__,__func__);
nmap <S-F8> i<END><cr>printf("[%s][%d]-[%s]\n",__FILE__,__LINE__,__func__); <ESC>
let Tlist_Show_One_File=0 "让taglist可以同时展示多个文件的函数列表
let Tlist_File_Fold_Auto_Close=1 "非当前文件，函数列表折叠隐藏
let Tlist_Exit_OnlyWindow=1 "当taglist是最后一个分割窗口时，自动推出vim
let Tlist_Auto_Open=1
let Tlist_Use_Left_Window = 1
"是否一直处理tags.1:处理;0:不处理
"let Tlist_Process_File_Always=1 "实时更新tags
let Tlist_Inc_Winwidth=0
