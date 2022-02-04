/*

\begin{itemize}
    \item $C_e$ est le coefficient d'efficacité,
    \item $R_g$ est la résistance limite d'un groupe de n pieux,
    \item $R_{b;i}$ est la résistance de pointe limite d'un pieu i du groupe supposé isolé,
    \item $R_{s;i}$ est la résistance limite par frottement axial d'un pieu i du groupe supposé isolé.
\end{itemize}

\begin{center}
    \begin{tabular}{|c|c|c|c|c|} 
        \hline
        proportion $\theta_1$ & $< 25\%$ & $< 33\%$ & $< 50\%$ & $> 50\%$ \\
        \hline
        $\phi<\SIlist{40}{\milli\metre}$ & $36\phi$ & $41\phi$ & $50\phi$ & $54\phi$ \\ 
        \hline
        $\phi=\SIlist{40}{\milli\metre}$ & $39\phi$ & $45\phi$ & $54\phi$ & $58\phi$ \\ 
        \hline
    \end{tabular}
\end{center}

\begin{figure}[h!]
	\centering
	\includegraphics[page=1, clip, trim= 17.68cm 2cm 12.5cm 5.61cm, width=1.2 \textwidth,height=1.2 \textheight,keepaspectratio,angle=90]{{"/Users/mnnguyen/Documents/Gcpm/users/ngoc2012/Opera/PieuxInd0/implantation_all"}.pdf}
	\vspace{-0.5cm}
	\caption{Fondation prévue par le bureau d’études structures}
	\label{image-fondationPrevu}
\end{figure}

*/

function checkCommentLine(line) {
  let re = /\s*%/g;
  return line.search(re) === 0
}

function findBeginGroupes(line) {
  let output = ""
  const groupesName = ['itemize', 'center', 'figure']
  groupesName.forEach(g => {
    let re = new RegExp("\\s*\\\\begin{" + g + "}")
    if (re.test(line)) {
      output = g
    }
  })
  return output
}

function findEndGroupes(line) {
  let output = ""
  const groupesName = ['itemize', 'center', 'figure']
  groupesName.forEach(g => {
    let re = new RegExp("\\s*\\end{" + g + "}")
    if (re.test(line)) {
      output = g
    }
  })
  return output
}

function data2Groupes(data) {
  let groupes = []
  let groupeLines = []
  let inGroupe = false

  data.split('\n').forEach(line => {
    let groupeBegin = findBeginGroupes(line)
    let groupeEnd = findEndGroupes(line)
    if (groupeBegin !== "") {
      inGroupe = true
      if (groupes.length > 0) {groupes[groupes.length-1].value = [...groupeLines]}
      groupes.push({"type": groupeBegin, "value": []})
      groupeLines = []
    } else if (groupeEnd !== "") {
      groupes[groupes.length-1].value = [...groupeLines]
      groupeLines = [];
      inGroupe = false;
    } else if ((!inGroupe) & (groupeLines.length === 0)) {
      groupes.push({"type": "line", "value": []})
      if (!checkCommentLine(line)) {groupeLines.push(line)}
    } else {
      if (!checkCommentLine(line)) {groupeLines.push(line)}
    }
  })
  if (groupeLines.length > 0) {
    groupes[groupes.length-1].value = [...groupeLines]
  }
  return groupes
}

export default data2Groupes;