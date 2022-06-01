#ifndef MC_STRUCTURES_H_
#define MC_STRUCTURES_H_


struct setpoint
{
    double pwm_frequency;
    double pwm_duty;
};

enum switch_mode {
	SM_OFF,
	SM_ON,
	SM_PWM
};

struct switch_modes {
	enum switch_mode hu;
	enum switch_mode lu;
	enum switch_mode hv;
	enum switch_mode lv;
	enum switch_mode hw;
	enum switch_mode lw;
};

/* Motor parameter struct. */
struct motor {
	double inertia;  /* aka 'J' in kg/(m^2) */
	double damping;  /* aka 'B' in Nm/(rad/s) */
	double static_friction; /* in Nm */
	double Kv;       /* motor constant in RPM/V */
	double L;        /* Coil inductance in H */
	double M;        /* Mutual coil inductance in H */
	double R;        /* Coil resistence in Ohm */
	double VDC;      /* Supply voltage */
	int NbPoles;     /* NbPoles / 2 = Number of pole pairs (you count the permanent magnets on the rotor to get NbPoles) */
};

/* State vector struct. */
struct state_vector {
	double theta; /* angle of the rotor */
	double omega; /* angular speed of the rotor */
	double iu;    /* phase u current */
	double iv;    /* phase v current */
	double iw;    /* phase w current */
};

/* Command vector struct. */
struct command_vector {
	struct ssn *hu; /* low side phase U switch */
	struct ssn *lu; /* high side phase U switch */
	struct ssn *hv; /* low side phase V switch */
	struct ssn *lv; /* high side phase V switch */
	struct ssn *hw; /* low side phase W switch */
	struct ssn *lw; /* high side phase W switch */

    bool u_h; /* low side phase U switch */
	bool u_l; /* high side phase U switch */
	bool v_h; /* low side phase V switch */
	bool v_l; /* high side phase V switch */
	bool w_h; /* low side phase W switch */
	bool w_l; /* high side phase W switch */
};


/* Perturbation vector struct. */
struct perturbation_vector {
	double torque; /* Torque applied to the shaft of the motor. */
};

/* Output vector struct. */
struct output_vector {
	double iu;    /* Phase U current. */
	double iv;    /* Phase V current. */
	double iw;    /* Phase W current. */
	double vu;    /* Phase U voltage. */
	double vv;    /* Phase V voltage. */
	double vw;    /* Phase W voltage. */
	double theta; /* Rotor angle. */
	double omega; /* Rotor speed. */
};

/* Parameter struct. */
struct parameters {
	struct motor *m;
	struct command_vector *cv;
	struct perturbation_vector *pv;
};

/* Simulator internal voltages vector. */
struct voltages_vector {
	double eu;
	double ev;
	double ew;
	double vu;
	double vv;
	double vw;
	double star;
};

/* Switch state node. */
struct ssn {
	double time;
	bool state;
	bool stop;
	struct ssn *next;
};

#endif
