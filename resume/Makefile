FN=Su_Resume

.PHONY: ${FN}.pdf
${FN}.pdf: ${FN}_CS.pdf ${FN}_PH.pdf

${FN}_CS.pdf: ${FN}.tex
	compiletex ${FN} xelatex
	mv ${FN}.pdf ${FN}_CS.pdf

${FN}_PH.pdf: ${FN}.tex
	sed 's/\\isCStrue/\\isCSfalse/' -i ${FN}.tex
	compiletex ${FN} xelatex
	mv ${FN}.pdf ${FN}_PH.pdf
	sed 's/\\isCSfalse/\\isCStrue/' -i ${FN}.tex
