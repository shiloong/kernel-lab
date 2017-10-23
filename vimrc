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
set cscopequickfix=s-,c-,d-,i-,t-,e-
