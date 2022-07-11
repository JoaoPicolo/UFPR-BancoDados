select
    count(c.c_custkey)
from
    CUSTOMER c
where
    not exists (
        select
            *
        from
            ORDERS o
        where
            c.c_custkey = o.o_custkey
    );