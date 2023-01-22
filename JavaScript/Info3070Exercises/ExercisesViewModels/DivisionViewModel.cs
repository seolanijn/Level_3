using ExercisesDAL;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Reflection;
using System.Threading.Tasks;

namespace ExercisesViewModels
{
    public class DivisionViewModel
    {
        readonly private DivisionDAO _dao;

        public int Id { get; set; }
        public string Name { get; set; }
        public string Timer { get; set; }

        // constructor
        public DivisionViewModel()
        {
            _dao = new DivisionDAO();
        }

        public async Task<List<DivisionViewModel>> GetAll()
        {
            List<DivisionViewModel> allVms = new List<DivisionViewModel>();
            try
            {
                List<Division> allDivisions = await _dao.GetAll();
                foreach (Division div in allDivisions)
                {
                    DivisionViewModel divVm = new DivisionViewModel
                    {
                        Id = div.Id,
                        Name = div.Name,
                        Timer = Convert.ToBase64String(div.Timer)
                    };
                    allVms.Add(divVm);
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Problem in " + GetType().Name + " " +
                    MethodBase.GetCurrentMethod().Name + " " + ex.Message);
                throw;
            }
            return allVms;
        }
    }
}
