import os
import re


def main():
    file_prefix = 'include/tefri'
    exluded_files = []
    header_guard_regex = r'#[A-Za-z]+( )*((//( )*)|(/\*( )*))?((TEFRI_[A-Za-z]+(_[A-Za-z]+)*)|(METAXXA))_H(( )*\*/)?'

    if not os.path.exists('release') or os.path.isfile('release'):
        os.makedirs('release')

    main_header_path = file_prefix + '/tefri.h'
    header_text = get_license_comment() + '\n\n'
    with open(main_header_path) as main_header:
        header_lines = process_header(main_header, file_prefix, exluded_files, header_guard_regex)

        header_lines = move_includes_to_top(header_lines)
        header_lines = distinct_rest_headers(header_lines)

        header_text += '\n'.join(header_lines)
    
    with open('release/tefri.hpp', 'w') as release_header:
        release_header.write(header_text)

def is_system_include(line: str):
    return bool(re.match(r'#( )*include( )*<([A-Za-z]|[0-9]|_|/|\\|\.)+>', line))

def is_local_include(line: str):
    return bool(re.match(r'#( )*include( )*"([A-Za-z]|[0-9]|_|/|\\|\.)+"', line))

def is_include(line: str):
    return is_system_include(line) or is_local_include(line)

def get_license_comment():
    license_text = ''
    license_lines = []
    with open('LICENSE') as license:
        license_text = license.read()
    license_lines = license_text.split('\n')
    for i in range(len(license_lines)):
        license_lines[i] = "// " + license_lines[i]
    license_text = '\n'.join(license_lines)
    return license_text

def distinct_rest_headers(lines: list):
    i = 0
    lines_len = len(lines)
    excluded_files = []

    while i < lines_len:
        if is_system_include(lines[i]):
            file_path_begin = lines[i].find('<') 
            file_path_end = lines[i].find('>')
            file_path = lines[i][file_path_begin+1:file_path_end] 

            if file_path in excluded_files:
                lines = lines[:i] + lines[i+1:]
                i -= 1
                lines_len -= 1
            excluded_files.append(file_path)

        i += 1
    return lines

def process_header(file, current_dir: str, exluded_files: list, header_guard_regex: str):
    text = file.read()
    lines = text.split('\n')

    lines = move_includes_to_top(lines)
    return expand_includes(lines, current_dir, exluded_files, header_guard_regex)

def move_includes_to_top(lines: list):
    last_include_index = 0

    for i, line in enumerate(lines):
        if is_include(line):
            last_include_index = i
        elif last_include_index != 0:
            break

    for i in range(last_include_index + 1, len(lines)):
        if is_include(lines[i]) and (not 'cxxabi' in lines[i]):
            lines = move_text_row(lines, i, last_include_index + 1)
            last_include_index += 1

    return lines

def expand_includes(lines: list, current_dir: str, exluded_files: list, header_guard_regex: str):
    new_current_dir = ''

    lines_len = len(lines)
    i = 0
    while i < lines_len:
        if is_local_include(lines[i]):
            file_path_begin = lines[i].find('"')
            file_path_end = lines[i].rfind('"')

            file_path = lines[i][file_path_begin + 1:file_path_end].replace(os.sep, '/')
            if '/' in file_path:
                last_slash_index = file_path.rfind('/')
                new_current_dir = current_dir + '/' + file_path[:last_slash_index]
                file_path = new_current_dir + '/' + file_path[last_slash_index + 1:]
            else:
                file_path = current_dir + '/' + file_path
                new_current_dir = current_dir

            file_path = os.path.abspath(file_path)

            if not (file_path in exluded_files):
                with open(file_path) as file:
                    lines_to_insert = process_header(file, new_current_dir, exluded_files, header_guard_regex)
                    lines = insert_lines(lines, lines_to_insert, i)

                    exluded_files.append(file_path)

                    i += (len(lines_to_insert) - 1)
                    lines_len = len(lines)
            else:
                lines = lines[:i] + lines[i+1:]
                i -= 1
                lines_len -= 1
        elif bool(re.match(header_guard_regex, lines[i])):
            lines = lines[:i] + lines[i+1:]
            i -= 1
            lines_len -= 1
        i += 1
    return lines

def move_text_row(lines: list, from_row: int, to_row: int):
    if from_row < to_row:
        return lines[:from_row] + lines[from_row + 1:to_row] + [lines[from_row]] + lines[to_row:]
    elif from_row > to_row:
        return lines[:to_row] + [lines[from_row]] + lines[to_row:from_row] + lines[from_row + 1:] 
    else:
        return lines 

def insert_lines(lines: list, lines_to_insert: list, index: int):
    return lines[0: index] + lines_to_insert + lines[index + 1:]

if __name__ == '__main__':
    main()