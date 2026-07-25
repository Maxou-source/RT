 #===============================================================================#
#								PROJECT INFOS									  #
 #===============================================================================#

NAME				=	RT

 #=================================================#
#					COMPILATION						#
 #=================================================#

# CC					=	clang++

# FLAGS				=	$(DBFLAGS) $(CFLAGS) $(INCLUDES_FLAGED) $(CDFLAGS)

# PKG_CONFIG = pkg-config
# CFLAGS				=	-gdwarf-4 -Wall -Wextra -Werror $(shell $(PKG_CONFIG) --cflags xcb) -fenable-matrix -lglfw -lGL -ldl -lpthread#-std=c++98 #-fsanitize=address 
# LDFLAGS = $(shell $(PKG_CONFIG) --libs xcb)
CC					=	clang++
CC_C				=   clang

FLAGS				=	$(DBFLAGS) $(CFLAGS) $(INCLUDES_FLAGED) $(CDFLAGS)

PKG_CONFIG = pkg-config
CFLAGS				=	-gdwarf-4  $(shell $(PKG_CONFIG) --cflags xcb) -fenable-matrix
LDFLAGS = $(shell $(PKG_CONFIG) --libs xcb) -lglfw -lGL -ldl -lpthread

DBFLAGS				=	-g3

CDFLAGS				=	-MMD -MP

INCLUDEFLAG			=	-I

INCLUDES_FLAGED		=	$(addprefix $(INCLUDEFLAG), $(INCLUDES))

 #=================================================#
#					DIRECTORIES						#
 #=================================================#

SRC_DIR				=	src/
OBJECTS_DIR			=	Object/

CLASSES_DIR			=	Classes/
PARSE_DIR			=	Parse/
TUPLE_DIR			=	Tuple/
METHOD_DIR			=	Methods/
OPENGL_HANDLING		=	OpenGLHandling/
XCB_DIR				=	XCB/
WORLD_DIR			=	World/
MATRIX_DIR			=	Matrix/
GLAD_DIR			=   glad/

INCLUDE_DIR			=	includes/

 #===============================================================================#
#								SOURCES											  #
 #===============================================================================#

INCLUDES			=	$(INCLUDE_DIR) \
							$(addprefix $(INCLUDE_DIR), \
							$(OBJECTS_DIR) \
							$(PARSE_DIR) \
							$(TUPLE_DIR) \
							$(XCB_DIR) \
							$(MATRIX_DIR) \
							$(WORLD_DIR) \
							$(OPENGL_HANDLING) \
							)

SRC_FILES_CPP			=	$(addprefix $(SRC_DIR), \
						maintest.cpp \
						$(SRC_CLASSES) \
						)

SRC_FILES_C			=	$(addprefix $(SRC_DIR), \
						$(SRC_GLAD) \
						)

SRC_CLASSES			=	$(addprefix $(CLASSES_DIR), \
						$(SRC_PARSING) \
						$(SRC_OBJECTS) \
						$(SRC_TUPLE) \
						$(SRC_XCB) \
						$(SRC_MATRIX) \
						$(SRC_WORLD) \
						$(SRC_OPENGLHANDLING) \
						)

SRC_WORLD			=	$(addprefix $(WORLD_DIR), \
						World.cpp \
						Intersection.cpp \
						Computations.cpp \
						Camera.cpp \
						)

SRC_MATRIX			=	$(addprefix $(MATRIX_DIR), \
						Matrix.cpp \
						)


SRC_OBJECTS			= $(addprefix $(OBJECTS_DIR), \
						AObject.cpp \
						Sphere.cpp \
						Light.cpp \
						)

SRC_TUPLE			= $(addprefix $(TUPLE_DIR), \
						Tuple.cpp \
						Ray.cpp \
						)

SRC_XCB				= $(addprefix $(XCB_DIR), \
						XCB.cpp \
						Image.cpp \
						)
SRC_PARSING			=	$(addprefix $(PARSE_DIR), \
						parse.cpp \
						)

SRC_GLAD			= $(addprefix $(GLAD_DIR), glad.c)

SRC_OPENGLHANDLING	= $(addprefix $(OPENGL_HANDLING), OpenGLObject.cpp)


 #=============================================================================#
#									OBJETS										#
 #=============================================================================#

OBJS_DIR	=	.objects/

OBJS_CPP	=	$(subst $(SRC_DIR), $(OBJS_DIR), $(SRC_FILES_CPP:%.cpp=%.o))
OBJS_C		=	$(subst $(SRC_DIR), $(OBJS_DIR), $(SRC_FILES_C:%.c=%.o))

OBJS		=	$(OBJS_CPP) $(OBJS_C)

DEPS		=	$(OBJS:%.o=%.d)

 #=============================================================================#
#									RULES										#
 #=============================================================================#

$(OBJS_DIR)%.o : $(SRC_DIR)%.cpp
				mkdir -p $(dir $@)
				$(CC) $(FLAGS) -c $< -o $@

$(OBJS_DIR)%.o : $(SRC_DIR)%.c
				mkdir -p $(dir $@)
				$(CC_C) $(FLAGS) -c $< -o $@

all : $(NAME)

$(NAME)		:	$(OBJS_DIR) $(OBJS) Makefile
				$(CC) $(FLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

$(OBJS_DIR) :
				mkdir $(OBJS_DIR)

clean		:
				rm -rf $(OBJS_DIR)

fclean		:	clean
				rm -rf $(NAME)

re			:	fclean all

-include $(DEPS)

.PHONY: all clean fclean re
