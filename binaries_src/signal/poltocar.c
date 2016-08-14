/* Copyright (c) 2002-2003 krzYszcz and others.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.  */

#include <math.h>
#include "m_pd.h"
#include "unstable/forky.h"

typedef struct _poltocar
{
    t_object x_obj;
    t_inlet *poltocar;
    t_outlet  *x_out2;
    t_glist  *x_glist;
} t_poltocar;

static t_class *poltocar_class;

static t_int *poltocar_perform(t_int *w)
{
    int nblock = (int)(w[1]);
    t_float *in1 = (t_float *)(w[2]);
    t_float *in2 = (t_float *)(w[3]);
    t_float *out1 = (t_float *)(w[4]);
    t_float *out2 = (t_float *)(w[5]);
    while (nblock--)
    {
	float am = *in1++, ph = *in2++;
	*out1++ = am * cosf(ph);
	*out2++ = am * sinf(ph);  /* CHECKED */
    }
    return (w + 6);
}

static t_int *poltocar_perform_no_in(t_int *w)
{
    int nblock = (int)(w[1]);
    t_float *out1 = (t_float *)(w[4]);
    t_float *out2 = (t_float *)(w[5]);
    while (nblock--)
    {
        *out1++ = *out2++ = 0;
    }
    return (w + 6);
}

static void poltocar_dsp(t_poltocar *x, t_signal **sp)
{
    if (forky_hasfeeders((t_object *)x, x->x_glist, 0, &s_signal))
	dsp_add(poltocar_perform, 5, sp[0]->s_n, sp[0]->s_vec,
		sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec);
    else dsp_add(poltocar_perform_no_in, 5, sp[0]->s_n, sp[0]->s_vec,
                 sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec);
}

static void *poltocar_new(void)
{
    t_poltocar *x = (t_poltocar *)pd_new(poltocar_class);
    inlet_new((t_object *)x, (t_pd *)x, &s_signal, &s_signal);
    outlet_new((t_object *)x, &s_signal);
    x->x_glist = canvas_getcurrent();
    x->x_out2 = outlet_new((t_object *)x, &s_signal);
    return (x);
}

void poltocar_tilde_setup(void)
{
    poltocar_class = class_new(gensym("poltocar~"),
            (t_newmethod)poltocar_new, 0,
            sizeof(t_poltocar), 0, 0);
    class_addmethod(poltocar_class, nullfn, gensym("signal"), 0);
    class_addmethod(poltocar_class, (t_method) poltocar_dsp, gensym("dsp"), 0);
}
