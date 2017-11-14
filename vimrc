syntax on
"set foldmethod=syntax
set ruler number
"set relativenumber

""" Linux Formatting
set cindent cinoptions=:0,l1,t0,g0,(0
set tabstop=8 shiftwidth=8 softtabstop=8 noexpandtab
"set textwidth=80

""" Linux Highlighting
set colorcolumn=80
"set cursorline
set listchars=tab:>-,eol:$,trail:~,nbsp:%
set hlsearch incsearch
set nobackup
colorscheme evening
autocmd InsertEnter * 2match ErrorMsg /\s\+\%#\@<!$/
autocmd InsertLeave * 2match ErrorMsg /\s\+$/

""" Linux Keywords
autocmd Syntax c,cpp syntax keyword cOperator likely unlikely
autocmd Syntax c,cpp syntax keyword cType u8 u16 u32 u64 s8 s16 s32 s64
autocmd Syntax c,cpp syntax keyword cType __u8 __u16 __u32 __u64 __s8 __s16 __s32 __s64

""" Quickfix
if has('cscope')
   set cscopetag
   if has('quickfix')
      set cscopequickfix=s-,c-,d-,i-,t-,e-
   endif
   if filereadable("cscope.out")
      exe "cs add cscope.out"
   endif
   set cscopeverbose
   " To do the first type of search, hit 'CTRL-\', followed by one of the
   " cscope search types above (s,g,c,t,e,f,i,d).  The result of your cscope
   " search will be displayed in the current window.  You can use CTRL-T to
   " go back to where you were before the search.
   "
   nmap <C-\>s :cs find s <C-R>=expand("<cword>")<CR><CR>
   nmap <C-\>g :cs find g <C-R>=expand("<cword>")<CR><CR>
   nmap <C-\>c :cs find c <C-R>=expand("<cword>")<CR><CR>
   nmap <C-\>t :cs find t <C-R>=expand("<cword>")<CR><CR>
   nmap <C-\>e :cs find e <C-R>=expand("<cword>")<CR><CR>
   nmap <C-\>f :cs find f <C-R>=expand("<cfile>")<CR><CR>
   nmap <C-\>i :cs find i ^<C-R>=expand("<cfile>")<CR>$<CR>
   nmap <C-\>d :cs find d <C-R>=expand("<cword>")<CR><CR>
endif
