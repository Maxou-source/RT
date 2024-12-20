 #===============================================================================#
#								PROJECT INFOS									  #
 #===============================================================================#

NAME				=	RT

 #=================================================#
#					COMPILATION						#
 #=================================================#

CC					=	c++

FLAGS				=	$(DBFLAGS) $(CFLAGS) $(INCLUDES_FLAGED) $(CDFLAGS)

CFLAGS				=	-gdwarf-4 -Wall -Wextra -Werror #-std=c++98 #-fsanitize=address

DBFLAGS				=	-g3

CDFLAGS				=	-MMD -MP

INCLUDEFLAG			=	-I

INCLUDES_FLAGED		=	$(addprefix $(INCLUDEFLAG), $(INCLUDES))

 #=================================================#
#					DIRECTORIES						#
 #=================================================#

SRC_DIR				=	src/
CLASSES_DIR			=	Classes/
REQUEST				=	Request/
PARSE_DIR			=	Parsing/
METHOD_DIR			=	Methods/
METADATA_DIR		=	MetaData/

INCLUDE_DIR			=	includes/

 #===============================================================================#
#								SOURCES											  #
 #===============================================================================#

# INCLUDES			=	$(INCLUDE_DIR) \
# 						$(addprefix $(INCLUDE_DIR), \
# 						$(REQUEST) \
# 						$(PARSE_DIR) \
# 						$(METHOD_DIR) \
# 						$(METADATA_DIR) \
# 						)

SRC_FILES			=	$(addprefix $(SRC_DIR), \
						main.cpp \
						)

# SRC_CLASSES			=	$(addprefix $(CLASSES_DIR), \
# 						$(SRC_REQUEST) \
# 						$(SRC_PARSING) \
# 						$(SRC_METHODS) \
# 						$(SRC_METADATA) \
# 						MessageContext.cpp \
# 						Whole.cpp \
# 						CGI.cpp \
# 						)

# SRC_METADATA		=	$(addprefix $(METADATA_DIR), \
# 						ServerData.cpp \
# 						)

# SRC_REQUEST			=	$(addprefix $(REQUEST), \
# 						Connexion.cpp \
# 						RequestHandler.cpp \
# 						)

# SRC_PARSING			=	$(addprefix $(PARSE_DIR), \
# 						FunctionWrapper.cpp \
# 						Parse.cpp \
# 						)

# SRC_METHODS			=	$(addprefix $(METHOD_DIR), \
# 						Method.cpp \
# 						Get.cpp \
# 						Post.cpp \
# 						Delete.cpp \
# 						)

# $(addprefix $(UTILS), FunctionWrapper.cpp)
# $(addprefix $(CLASSES_DIR), Func.cpp)

 #=============================================================================#
#									OBJETS										#
 #=============================================================================#

OBJS_DIR	=	.objects/

OBJS		=	$(subst $(SRC_DIR), $(OBJS_DIR), $(SRC_FILES:%.cpp=%.o))

DEPS		=	$(OBJS:%.o=%.d)

 #=============================================================================#
#									RULES										#
 #=============================================================================#

$(OBJS_DIR)%.o : $(SRC_DIR)%.cpp
				mkdir -p $(dir $@)
				$(CC) $(FLAGS) -c $< -o $@

all : $(NAME)

$(NAME)		:	$(OBJS_DIR) $(OBJS) Makefile
				$(CC) $(FLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR) :
				mkdir $(OBJS_DIR)

clean		:
				rm -rf $(OBJS_DIR)

fclean		:	clean
				rm -rf $(NAME)

re			:	fclean all

-include $(DEPS)

.PHONY: all clean fclean re
