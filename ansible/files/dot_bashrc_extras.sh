
# Used to add extra entries to the path
pathmunge () {
    case ":${PATH}:" in
        *:"$1":*)
            ;;
        *)
            if [ "$2" = "after" ] ; then
                PATH=$PATH:$1
            else
                PATH=$1:$PATH
            fi
    esac
}

# Add extra directories to path
pathmunge ${HOME}/.local/bin

# So we can see snap binaries later
pathmunge /snap/bin
