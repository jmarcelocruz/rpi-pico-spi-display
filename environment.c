void *memcpy(void *s, const void *t, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        ((char *)s)[i] = ((char *)t)[i];
    }

    return s;
}

void *memmove(void *s, const void *t, unsigned int n) {
    return memcpy(s, t, n);
}

int memcmp(const void *s, const void *t, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        if (((char *)s)[i] != ((char *)t)[i]) {
            return ((char *)s)[i] - ((char *)t)[i];
        }
    }

    return 0;
}

void *memset(void *s, unsigned char c, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        ((char *)s)[i] = c;
    }

    return s;
}
