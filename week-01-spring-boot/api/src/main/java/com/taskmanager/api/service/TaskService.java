package com.taskmanager.api.service;

import com.taskmanager.api.model.Task;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

@Service
public class TaskService {
    private List<Task> tasks = new ArrayList<>();

    public TaskService() {
        tasks.add(new Task( "Master Spring Boot-1", "Learn annotations and MVC architecture", Task.Status.DONE));
        tasks.add(new Task( "Master Spring Boot-2", "Learn annotations and MVC architecture", Task.Status.IN_PROGRESS));
        tasks.add(new Task( "Review", "Review what I have learned", Task.Status.TODO));
        tasks.add(new Task( "Submit", "Send the project to friends", Task.Status.SUSPENDED));
    }

    public List<Task> getAllTasks(){
        return tasks;
    }

    public Optional<Task> getTaskByID(Long id) {
        return tasks.stream()
                .filter(task -> task.getId().equals(id))
                .findFirst();
    }
    public Task addTask(Task task){
        tasks.add(task);
        return task;
    }
    public Optional<Task> updateTask(Long id , Task updatedTask) {
        for (Task task : tasks) {
            if (task.getId().equals(id)) {
                task.setTitle(updatedTask.getTitle());
                task.setDescription(updatedTask.getDescription());
                task.setStatus(updatedTask.getStatus());
                return Optional.of(task);
            }
        }
        return Optional.empty();
    }
    public boolean deleteTask(Long id){
        return  tasks.removeIf(task -> task.getId().equals(id));
    }
}
