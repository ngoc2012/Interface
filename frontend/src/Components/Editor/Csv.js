import React from 'react';
import TableBasic from '../Table/TableBasic';

function Csv(props) {

  return (
    <div>
      <TableBasic {...props} rows={props.rows} cols={props.cols} filtred={true} indexed={true} tools={true}></TableBasic>
    </div>
  )
}

export default Csv;