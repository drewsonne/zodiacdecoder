// constants

const int		kMaxAlphaChar = 26;
const int		kMaxCipherChar = 63;
const double	kLagrangeValue = 0.01f;					// for Lagrangian smoothing during normalization
const int		kEMiterations = 1;						// iterations of the main loop
const int		kBoundaryChar = kMaxAlphaChar;
const double	kMinTolerance = 0.00001f;
const double	kInitSTStartValue = 1.0f;				// the unifrm value for filling the initial solution table = 1/26
const bool		getHighestProbableKeyPerColumn = true;	// determines which function for key selection is called
