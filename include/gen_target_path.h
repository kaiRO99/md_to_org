/**
 * @file gen_target_path.h
 * @brief Generate the target path based on input path.
 * @author Kai Ryall Ota
 * @date March 2026
 * */

#ifndef GEN_TARGET_PATH_H_
#define GEN_TARGET_PATH_H_
/**
 * @brief Generate output file in same directory as input file.(".md"->".org")
 * @param{*char} input_path
 * @return{char*} Filepath of target file.
 * */
char *generate_target_path(const char *input_path);

#endif // GEN_TARGET_PATH_H_
