if !has('python')
    finish
endif

highlight currawong ctermbg=darkred guibg=darkred

function! s:Check25Lines()
python << EOF
import vim

long_functions = []

for i, line in enumerate(vim.current.buffer):
    try:
        if line[0] == '{':
            start = i
            empty_lines = 0
        elif len(line) <= 1:
            empty_lines += 1
        elif line[0] == '}':
            end = i
            if end - start - empty_lines >= 25:
                long_functions.append(start)
    except:
        pass

vim.command("match currawong /\%{}l/".format("l|".join(map(str,
    long_functions))))
EOF
endfunc

autocmd BufWritePre * :call s:Check25Lines()
